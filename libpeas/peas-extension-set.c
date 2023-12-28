/*
 * peas-extension-set.c
 * This file is part of libpeas
 *
 * Copyright (C) 2010 Steve Frécinaux
 *
 * libpeas is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libpeas is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "config.h"

#include <string.h>

#include "peas-extension-set.h"

#include "peas-engine-priv.h"
#include "peas-i18n-priv.h"
#include "peas-marshal.h"
#include "peas-plugin-info.h"
#include "peas-utils.h"

/**
 * PeasExtensionSet:
 *
 * Proxy for a set of extensions of the same type.
 *
 * A #PeasExtensionSet is an object which proxies method calls to a set
 * of actual extensions.  The application writer will use these objects
 * in order to call methods on several instances of an actual extension
 * exported by all the currently loaded plugins.
 *
 * #PeasExtensionSet will automatically track loading and unloading of
 * the plugins, and signal appearance and disappearance of new
 * extension instances.  You should connect to those signals if you
 * wish to call specific methods on loading or unloading time.
 *
 * Here is the code for a typical setup of #PeasExtensionSet with
 * ExampleActivatable as the watched extension point, and GtkWindow
 * instances as the target objects:
 *
 * ```c
 * static void
 * on_extension_added (PeasExtensionSet   *set,
 *                     PeasPluginInfo     *info,
 *                     ExampleActivatable *activatable)
 * {
 *   example_activatable_activate (activatable);
 * }
 *
 * static void
 * on_extension_removed (PeasExtensionSet   *set,
 *                       PeasPluginInfo     *info,
 *                       ExampleActivatable *activatable)
 * {
 *   example_activatable_deactivate (activatable);
 * }
 *
 * PeasExtensionSet *
 * setup_extension_set (PeasEngine *engine,
 *                      GtkWindow  *window)
 * {
 *   PeasExtensionSet *set;
 *
 *   set = peas_extension_set_new (engine, EXAMPLE_TYPE_ACTIVATABLE,
 *                                 "object", window, NULL);
 *   peas_extension_set_foreach (set,
 *                               (PeasExtensionSetForeachFunc) on_extension_added,
 *                               NULL);
 *   g_signal_connect (set, "extension-added",
 *                     G_CALLBACK (on_extension_added), NULL);
 *   g_signal_connect (set, "extension-removed",
 *                     G_CALLBACK (on_extension_removed), NULL);
 *   return set;
 * }
 * ```
 **/

G_GNUC_BEGIN_IGNORE_DEPRECATIONS

struct _PeasExtensionSet
{
  GObject parent_instance;

  PeasEngine *engine;
  GType exten_type;
  guint n_parameters;

  GParameter *parameters;

  GQueue extensions;
};

typedef struct {
  PeasPluginInfo *info;
  GObject *exten;
} ExtensionItem;

typedef struct {
  guint n_parameters;

  GParameter *parameters;
} PeasParameterArray;

G_GNUC_END_IGNORE_DEPRECATIONS

/* Signals */
enum {
  EXTENSION_ADDED,
  EXTENSION_REMOVED,
  LAST_SIGNAL
};

/* Properties */
enum {
  PROP_0,
  PROP_ENGINE,
  PROP_EXTENSION_TYPE,
  PROP_CONSTRUCT_PROPERTIES,
  N_PROPERTIES
};

static guint signals[LAST_SIGNAL];
static GParamSpec *properties[N_PROPERTIES] = { NULL };

static void list_model_iface_init (GListModelInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (PeasExtensionSet, peas_extension_set, G_TYPE_OBJECT,
                               G_IMPLEMENT_INTERFACE (G_TYPE_LIST_MODEL, list_model_iface_init))

static void
set_construct_properties (PeasExtensionSet   *set,
                          PeasParameterArray *array)
{
  set->n_parameters = array->n_parameters;

  G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  set->parameters = g_new0 (GParameter, array->n_parameters);
  G_GNUC_END_IGNORE_DEPRECATIONS

  for (guint i = 0; i < array->n_parameters; i++)
    {
      set->parameters[i].name = g_intern_string (array->parameters[i].name);
      g_value_init (&set->parameters[i].value, G_VALUE_TYPE (&array->parameters[i].value));
      g_value_copy (&array->parameters[i].value, &set->parameters[i].value);
    }
}

static void
peas_extension_set_set_property (GObject      *object,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (object);

  switch (prop_id)
    {
    case PROP_ENGINE:
      set->engine = g_value_get_object (value);
      break;
    case PROP_EXTENSION_TYPE:
      set->exten_type = g_value_get_gtype (value);
      break;
    case PROP_CONSTRUCT_PROPERTIES:
      set_construct_properties (set, g_value_get_pointer (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
peas_extension_set_get_property (GObject    *object,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (object);

  switch (prop_id)
    {
    case PROP_ENGINE:
      g_value_set_object (value, set->engine);
      break;
    case PROP_EXTENSION_TYPE:
      g_value_set_gtype (value, set->exten_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
add_extension (PeasExtensionSet *set,
               PeasPluginInfo   *info)
{
  GObject *exten;
  ExtensionItem *item;
  guint position;

  /* Let's just ignore unloaded plugins... */
  if (!peas_plugin_info_is_loaded (info))
    return;

  if (!peas_engine_provides_extension (set->engine, info,
                                       set->exten_type))
    return;

  exten = _peas_engine_create_extensionv (set->engine, info,
                                         set->exten_type,
                                         set->n_parameters,
                                         set->parameters);

  item = g_slice_new (ExtensionItem);
  item->info = info;
  item->exten = exten;

  position = set->extensions.length;

  g_queue_push_tail (&set->extensions, item);

  g_list_model_items_changed (G_LIST_MODEL (set), position, 0, 1);
  g_signal_emit (set, signals[EXTENSION_ADDED], 0, info, exten);
}

static void
remove_extension_item (PeasExtensionSet *set,
                       ExtensionItem    *item)
{
  g_signal_emit (set, signals[EXTENSION_REMOVED], 0, item->info, item->exten);

  g_object_unref (item->exten);

  g_slice_free (ExtensionItem, item);
}

static void
remove_extension (PeasExtensionSet *set,
                  PeasPluginInfo   *info)
{
  guint position = 0;

  for (GList *l = set->extensions.head; l != NULL; l = l->next, position++)
    {
      ExtensionItem *item = l->data;

      if (item->info != info)
        continue;

      remove_extension_item (set, item);
      g_queue_delete_link (&set->extensions, l);
      g_list_model_items_changed (G_LIST_MODEL (set), position, 1, 0);
      return;
    }
}

static void
peas_extension_set_init (PeasExtensionSet *set)
{
  g_queue_init (&set->extensions);
}

static void
peas_extension_set_constructed (GObject *object)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (object);
  guint n_items;

  if (set->engine == NULL)
    set->engine = peas_engine_get_default ();

  g_object_ref (set->engine);

  n_items = g_list_model_get_n_items (G_LIST_MODEL (set->engine));

  for (guint i = 0; i < n_items; i++)
    {
      PeasPluginInfo *info = g_list_model_get_item (G_LIST_MODEL (set->engine), i);
      add_extension (set, info);
      g_object_unref (info);
    }

  g_signal_connect_object (set->engine, "load-plugin",
                           G_CALLBACK (add_extension), set,
                           G_CONNECT_AFTER | G_CONNECT_SWAPPED);
  g_signal_connect_object (set->engine, "unload-plugin",
                           G_CALLBACK (remove_extension), set,
                           G_CONNECT_SWAPPED);

  G_OBJECT_CLASS (peas_extension_set_parent_class)->constructed (object);
}

static void
peas_extension_set_dispose (GObject *object)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (object);

  if (set->extensions.length > 0)
    {
      for (const GList *l = set->extensions.tail; l != NULL; l = l->prev)
        remove_extension_item (set, l->data);

      g_queue_clear (&set->extensions);
    }

  if (set->parameters != NULL)
    {
      while (set->n_parameters-- > 0)
        g_value_unset (&set->parameters[set->n_parameters].value);

      g_free (set->parameters);
      set->parameters = NULL;
    }

  g_clear_object (&set->engine);

  G_OBJECT_CLASS (peas_extension_set_parent_class)->dispose (object);
}

static void
peas_extension_set_class_init (PeasExtensionSetClass *klass)
{
  GType the_type = G_TYPE_FROM_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = peas_extension_set_set_property;
  object_class->get_property = peas_extension_set_get_property;
  object_class->constructed = peas_extension_set_constructed;
  object_class->dispose = peas_extension_set_dispose;

  /**
   * PeasExtensionSet::extension-added:
   * @set: A #PeasExtensionSet.
   * @info: A #PeasPluginInfo.
   * @extension: A #PeasExtension.
   *
   * Emitted when a new extension has been added to the #PeasExtensionSet.
   *
   * It happens when a new plugin implementing the extension set's extension
   * type is loaded.
   *
   * You should connect to this signal in order to set up the extensions when
   * they are loaded. Note that this signal is not fired for extensions coming
   * from plugins that were already loaded when the #PeasExtensionSet instance
   * was created. You should set those up by yourself.
   */
  signals[EXTENSION_ADDED] =
    g_signal_new (I_("extension-added"),
                  the_type,
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  peas_cclosure_marshal_VOID__OBJECT_OBJECT,
                  G_TYPE_NONE,
                  2,
                  PEAS_TYPE_PLUGIN_INFO | G_SIGNAL_TYPE_STATIC_SCOPE,
                  G_TYPE_OBJECT);
  g_signal_set_va_marshaller (signals[EXTENSION_ADDED],
                              G_TYPE_FROM_CLASS (klass),
                              peas_cclosure_marshal_VOID__OBJECT_OBJECTv);

  /**
   * PeasExtensionSet::extension-removed:
   * @set: A #PeasExtensionSet.
   * @info: A #PeasPluginInfo.
   * @extension: A #PeasExtension.
   *
   * Emitted when a new extension is about to be removed from the
   * #PeasExtensionSet.
   *
   * It happens when a plugin implementing the extension set's extension type is
   * unloaded, or when the #PeasExtensionSet itself is destroyed.
   *
   * You should connect to this signal in order to clean up the extensions
   * when their plugin is unload. Note that this signal is not fired for the
   * [class@GObject.Object] instances still available when the #PeasExtensionSet
   * instance is destroyed. You should clean those up by yourself.
   */
  signals[EXTENSION_REMOVED] =
    g_signal_new (I_("extension-removed"),
                  the_type,
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  peas_cclosure_marshal_VOID__OBJECT_OBJECT,
                  G_TYPE_NONE,
                  2,
                  PEAS_TYPE_PLUGIN_INFO | G_SIGNAL_TYPE_STATIC_SCOPE,
                  G_TYPE_OBJECT);
  g_signal_set_va_marshaller (signals[EXTENSION_REMOVED],
                              G_TYPE_FROM_CLASS (klass),
                              peas_cclosure_marshal_VOID__OBJECT_OBJECTv);

  properties[PROP_ENGINE] =
    g_param_spec_object ("engine",
                         "Engine",
                         "The PeasEngine this set is attached to",
                         PEAS_TYPE_ENGINE,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  properties[PROP_EXTENSION_TYPE] =
    g_param_spec_gtype ("extension-type",
                        "Extension Type",
                        "The extension GType managed by this set",
                        G_TYPE_NONE,
                        G_PARAM_READWRITE |
                        G_PARAM_CONSTRUCT_ONLY |
                        G_PARAM_STATIC_STRINGS);

  properties[PROP_CONSTRUCT_PROPERTIES] =
    g_param_spec_pointer ("construct-properties",
                          "Construct Properties",
                          "The properties to pass the extensions when creating them",
                          G_PARAM_WRITABLE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/**
 * peas_extension_set_get_extension:
 * @set: A #PeasExtensionSet
 * @info: a #PeasPluginInfo
 *
 * Returns the [class@GObject.Object] object corresponding to @info.
 *
 * If the plugin doesn't provide such an extension, it returns %NULL.
 *
 * Returns: (transfer none) (nullable): a reference to a #GObject
 */
GObject *
peas_extension_set_get_extension (PeasExtensionSet *set,
                                  PeasPluginInfo   *info)
{
  g_return_val_if_fail (PEAS_IS_EXTENSION_SET (set), NULL);
  g_return_val_if_fail (info != NULL, NULL);

  for (const GList *l = set->extensions.head; l != NULL; l = l->next)
    {
      ExtensionItem *item = l->data;

      if (item->info == info)
        return item->exten;
    }

  return NULL;
}

/**
 * peas_extension_set_foreach:
 * @set: A #PeasExtensionSet.
 * @func: (scope call): A function call for each extension.
 * @data: Optional data to be passed to the function or %NULL.
 *
 * Calls @func for each [class@GObject.Object].
 */
void
peas_extension_set_foreach (PeasExtensionSet            *set,
                            PeasExtensionSetForeachFunc  func,
                            gpointer                     data)
{
  g_return_if_fail (PEAS_IS_EXTENSION_SET (set));
  g_return_if_fail (func != NULL);

  for (const GList *l = set->extensions.head; l != NULL; l = l->next)
    {
      ExtensionItem *item = (ExtensionItem *) l->data;

      func (set, item->info, item->exten, data);
    }
}

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
static PeasExtensionSet *
peas_extension_set_newv (PeasEngine *engine,
                         GType       exten_type,
                         guint       n_parameters,
                         GParameter *parameters)
{
  PeasParameterArray construct_properties = { n_parameters, parameters };

  g_return_val_if_fail (engine == NULL || PEAS_IS_ENGINE (engine), NULL);
  g_return_val_if_fail (G_TYPE_IS_INTERFACE (exten_type) ||
                        G_TYPE_IS_ABSTRACT (exten_type), NULL);

  return PEAS_EXTENSION_SET (g_object_new (PEAS_TYPE_EXTENSION_SET,
                                           "engine", engine,
                                           "extension-type", exten_type,
                                           "construct-properties", &construct_properties,
                                           NULL));
}

/**
 * peas_extension_set_new_with_properties: (rename-to peas_extension_set_new)
 * @engine: (allow-none): A #PeasEngine, or %NULL.
 * @exten_type: the extension #GType.
 * @n_properties: the length of the @prop_names and @prop_values array.
 * @prop_names: (array length=n_properties): an array of property names.
 * @prop_values: (array length=n_properties): an array of property values.
 *
 * Create a new #PeasExtensionSet for the @exten_type extension type.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * See [ctor@ExtensionSet.new] for more information.
 *
 * Returns: (transfer full): a new instance of #PeasExtensionSet.
 */
PeasExtensionSet *
peas_extension_set_new_with_properties (PeasEngine    *engine,
                                        GType          exten_type,
                                        guint          n_properties,
                                        const char   **prop_names,
                                        const GValue  *prop_values)
{
  PeasExtensionSet *ret;
  PeasParameterArray construct_properties;
  GParameter *parameters = NULL;

  g_return_val_if_fail (engine == NULL || PEAS_IS_ENGINE (engine), NULL);
  g_return_val_if_fail (G_TYPE_IS_INTERFACE (exten_type) ||
                        G_TYPE_IS_ABSTRACT (exten_type), NULL);
  g_return_val_if_fail (n_properties == 0 || prop_names != NULL, NULL);
  g_return_val_if_fail (n_properties == 0 || prop_values != NULL, NULL);

  if (n_properties > 0)
    {
      parameters = g_new0 (GParameter, n_properties);
      if (!peas_utils_properties_array_to_parameter_list (exten_type,
                                                          n_properties,
                                                          prop_names,
                                                          prop_values,
                                                          parameters))
        {
          /* Already warned */
          g_free (parameters);
          return NULL;
        }
    }

  construct_properties.n_parameters = n_properties;
  construct_properties.parameters = parameters;

  ret = g_object_new (PEAS_TYPE_EXTENSION_SET,
                      "engine", engine,
                      "extension-type", exten_type,
                      "construct-properties", &construct_properties,
                      NULL);

  g_free (parameters);
  return ret;
}

/**
 * peas_extension_set_new_valist: (skip)
 * @engine: (allow-none): A #PeasEngine, or %NULL.
 * @exten_type: the extension #GType.
 * @first_property: the name of the first property.
 * @var_args: the value of the first property, followed optionally by more
 *   name/value pairs, followed by %NULL.
 *
 * Create a new #PeasExtensionSet for the @exten_type extension type.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * See [ctor@ExtensionSet.new] for more information.
 *
 * Returns: a new instance of #PeasExtensionSet.
 */
PeasExtensionSet *
peas_extension_set_new_valist (PeasEngine *engine,
                               GType       exten_type,
                               const char *first_property,
                               va_list     var_args)
{
  GParameter *parameters;
  guint n_parameters;
  PeasExtensionSet *set;

  g_return_val_if_fail (engine == NULL || PEAS_IS_ENGINE (engine), NULL);
  g_return_val_if_fail (G_TYPE_IS_INTERFACE (exten_type) ||
                        G_TYPE_IS_ABSTRACT (exten_type), NULL);

  if (!peas_utils_valist_to_parameter_list (exten_type, first_property,
                                            var_args, &parameters,
                                            &n_parameters))
    {
      /* Already warned */
      return NULL;
    }

  set = peas_extension_set_newv (engine, exten_type, n_parameters, parameters);

  while (n_parameters-- > 0)
    g_value_unset (&parameters[n_parameters].value);
  g_free (parameters);

  return set;
}

/**
 * peas_extension_set_new: (skip)
 * @engine: (allow-none): A #PeasEngine, or %NULL.
 * @exten_type: the extension #GType.
 * @first_property: the name of the first property.
 * @...: the value of the first property, followed optionally by more
 *   name/value pairs, followed by %NULL.
 *
 * Create a new #PeasExtensionSet for the @exten_type extension type.
 *
 * At any moment, the #PeasExtensionSet will contain an extension instance for
 * each loaded plugin which implements the @exten_type extension type. It does
 * so by connecting to the relevant signals from [class@Engine].
 *
 * The property values passed to peas_extension_set_new() will be used for the
 * construction of new extension instances.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * See [method@Engine.create_extension] for more information.
 *
 * Returns: a new instance of #PeasExtensionSet.
 */
PeasExtensionSet *
peas_extension_set_new (PeasEngine *engine,
                        GType       exten_type,
                        const char *first_property,
                        ...)
{
  va_list var_args;
  PeasExtensionSet *set;

  g_return_val_if_fail (engine == NULL || PEAS_IS_ENGINE (engine), NULL);
  g_return_val_if_fail (G_TYPE_IS_INTERFACE (exten_type) ||
                        G_TYPE_IS_ABSTRACT (exten_type), NULL);

  va_start (var_args, first_property);
  set = peas_extension_set_new_valist (engine, exten_type, first_property, var_args);
  va_end (var_args);

  return set;
}
G_GNUC_END_IGNORE_DEPRECATIONS

static guint
peas_extension_set_get_n_items (GListModel *model)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (model);

  return set->extensions.length;
}

static gpointer
peas_extension_set_get_item (GListModel *model,
                             guint       position)
{
  PeasExtensionSet *set = PEAS_EXTENSION_SET (model);
  ExtensionItem *item;

  if (position >= set->extensions.length)
    return NULL;

  item = g_queue_peek_nth (&set->extensions, position);
  g_assert (item != NULL);

  return g_object_ref (item->exten);
}

static GType
peas_extension_set_get_item_type (GListModel *model)
{
  return G_TYPE_OBJECT;
}

static void
list_model_iface_init (GListModelInterface *iface)
{
  iface->get_item_type = peas_extension_set_get_item_type;
  iface->get_item = peas_extension_set_get_item;
  iface->get_n_items = peas_extension_set_get_n_items;
}
