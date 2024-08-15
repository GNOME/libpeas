/*
 * peas-plugin-loader-gjs.cpp
 * This file is part of libpeas
 *
 * Copyright 2023 Christian Hergert <chergert@redhat.com>
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
 */

#include "config.h"

#include <memory>
#include <vector>

#include <js/Array.h>
#include <js/CompilationAndEvaluation.h>
#include <js/CompileOptions.h>
#include <js/Conversions.h>
#include <js/ErrorReport.h>
#include <js/Exception.h>
#include <js/Object.h>
#include <js/SourceText.h>
#include <js/TypeDecls.h>
#include <jsapi.h>
#include <mozilla/Utf8.h>

#include <gjs/gjs.h>

#include <jsapi.h>

#include <js/Modules.h>
#include <js/String.h>

#include "peas-plugin-loader-gjs.h"

#include "libpeas/peas-plugin-info-priv.h"

struct _PeasPluginLoaderGjs {
  PeasPluginLoader parent_instance;
  GjsContext *context;
};

typedef struct _PeasPluginLoaderGjsData
{
  PeasPluginLoaderGjs *loader;
  GArray *gtypes;
  const char *module_name;
} PeasPluginLoaderGjsData;

G_DEFINE_FINAL_TYPE (PeasPluginLoaderGjs, peas_plugin_loader_gjs, PEAS_TYPE_PLUGIN_LOADER)

static GQuark quark_extension_type;
static const char *intern_plugin_info;
static GHashTable *gjs_loader_info;
static const JSClass g_PeasLoader = { "PeasLoader", JSCLASS_GLOBAL_FLAGS, &JS::DefaultGlobalClassOps };
static const size_t GTYPE_SLOT = 0;  // index of GJS's GType in JS object's private slots

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              PEAS_TYPE_PLUGIN_LOADER,
                                              PEAS_TYPE_PLUGIN_LOADER_GJS);
}

static void
report_thrown (JSContext  *cx,
               const char *message)
{
  JS::ExceptionStack thrown{cx};
  JS::StealPendingExceptionStack (cx, &thrown);
  JS::ErrorReportBuilder report{cx};
  report.init (cx, thrown,
               JS::ErrorReportBuilder::SniffingBehavior::WithSideEffects);
  g_critical ("%s: %s", message, report.toStringResult ().c_str ());
}

static void
obtain_gtypes (GjsContext *gjs,
               void       *loader_data_ptr)
{
  PeasPluginLoaderGjsData *loader_data = (PeasPluginLoaderGjsData *)loader_data_ptr;

  auto *cx = static_cast<JSContext*>(gjs_context_get_native_context (gjs));

  JS::CompileOptions options{cx};
  options.setFileAndLine("getGTypes", 1);

  static const char *import_template = R"js(
  {
    const loop = new imports.gi.GLib.MainLoop(null, false);
    const gtypes = [];
    let error;
    import('%s')
    .then(importedModule => {
        for (const exportedValue of Object.values(importedModule)) {
            if ((typeof exportedValue === 'object' ||
                    typeof exportedValue === 'function') &&
                exportedValue !== null &&
                '$gtype' in exportedValue) {
                gtypes.push(exportedValue.$gtype);
            }
        }
    })
    .catch(e => (error = e))
    .finally(() => loop.quit());
    loop.run();
    if (error)
        throw error;
    gtypes;
  }
  )js";

  std::unique_ptr<char, decltype(g_free) *>
    code{g_strdup_printf (import_template, loader_data->module_name), g_free};

  JS::SourceText<mozilla::Utf8Unit> source;
  if (!source.init (cx, code.get(), strlen (code.get()), JS::SourceOwnership::Borrowed))
    {
      report_thrown (cx, "Error storing getGTypes");
      return;
    }

  JS::RootedValue gtypes_val{cx};
  if (!JS::Evaluate(cx, options, source, &gtypes_val))
    {
      report_thrown(cx, "Error executing getGTypes");
      return;
    }

  if (!gtypes_val.isObject())
    {
      g_critical ("Unexpected return value from getGTypes");
      return;
    }

  JS::RootedObject gtypes{cx, &gtypes_val.toObject()};
  bool is_array;
  uint32_t gtypes_len;
  if (!JS::IsArrayObject (cx, gtypes, &is_array) ||
      !JS::GetArrayLength (cx, gtypes, &gtypes_len))
    {
      report_thrown (cx, "Error reading array from getGTypes");
      return;
    }
  if (!is_array)
    {
      g_critical ("Weird return value from getGTypes");
      return;
    }

  JS::RootedValue gtype_val{cx};
  for (uint32_t ix = 0; ix < gtypes_len; ix++) {
    if (!JS_GetElement(cx, gtypes, ix, &gtype_val))
        report_thrown(cx, "Error getting element from GType array");
    if (!gtype_val.isObject())
        g_error("Weird value in GType array");
    GType gtype = GPOINTER_TO_SIZE(JS::GetMaybePtrFromReservedSlot<void>(
        &gtype_val.toObject(), GTYPE_SLOT));
    g_array_append_val (loader_data->gtypes, gtype);
  }
}

static gboolean
peas_plugin_loader_gjs_provides_extension (PeasPluginLoader *loader,
                                           PeasPluginInfo   *info,
                                           GType             exten_type)
{
  PeasPluginLoaderGjsData *data = (PeasPluginLoaderGjsData *)info->loader_data;

  for (guint i = 0; i < data->gtypes->len; i++)
    {
      GType gtype = g_array_index (data->gtypes, GType, i);

      if (g_type_is_a (gtype, exten_type))
        return TRUE;
    }

  return FALSE;
}

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
static GObject *
peas_plugin_loader_gjs_create_extension (PeasPluginLoader *loader,
                                         PeasPluginInfo   *info,
                                         GType             exten_type,
                                         guint             n_parameters,
                                         GParameter       *parameters)
{
  PeasPluginLoaderGjsData *loader_data = (PeasPluginLoaderGjsData *)info->loader_data;
  static GType plugin_info_type = G_TYPE_INVALID;
  GObjectClass *object_class;
  GParamSpec *pspec;
  GObject *object = NULL;
  GType the_type = G_TYPE_INVALID;

  if G_UNLIKELY (plugin_info_type == G_TYPE_INVALID)
    plugin_info_type = PEAS_TYPE_PLUGIN_INFO;

  for (guint i = 0; i < loader_data->gtypes->len; i++)
    {
      GType gtype = g_array_index (loader_data->gtypes, GType, i);

      if (g_type_is_a (gtype, exten_type))
        {
          the_type = gtype;
          break;
        }
    }

  if (the_type == G_TYPE_INVALID)
    goto out;

  object_class = (GObjectClass *)g_type_class_ref (the_type);
  pspec = g_object_class_find_property (object_class, intern_plugin_info);

  if (pspec == NULL || pspec->value_type != plugin_info_type)
    {
      object = (GObject *)g_object_newv (the_type, n_parameters, parameters);
    }
  else
    {
      GParameter *exten_parameters;

      /* We want to add a "plugin-info" property so we can pass it to
       * the extension if it inherits from PeasExtensionBase. No need to
       * actually "duplicate" the GValues, a memcpy is sufficient as the
       * source GValues are longer lived than our local copy.
       */
      exten_parameters = g_newa (GParameter, n_parameters + 1);
      if (n_parameters > 0)
        memcpy (exten_parameters, parameters, sizeof (GParameter) * n_parameters);

      /* Initialize our additional property.
       * If the instance does not have a plugin-info property
       * then PeasObjectModule will remove the property.
       */
      exten_parameters[n_parameters].name = intern_plugin_info;
      memset (&exten_parameters[n_parameters].value, 0, sizeof (GValue));
      g_value_init (&exten_parameters[n_parameters].value, PEAS_TYPE_PLUGIN_INFO);
      g_value_set_object (&exten_parameters[n_parameters].value, info);
      object = (GObject *)g_object_newv (the_type, n_parameters + 1, exten_parameters);
      g_value_unset (&exten_parameters[n_parameters].value);
    }

  g_type_class_unref (object_class);

  if (object == NULL)
    goto out;

  /* Sink floating references if necessary */
  if (g_object_is_floating (object))
    g_object_ref_sink (object);

  /* We have to remember which interface we are instantiating
   * for the deprecated peas_extension_get_extension_type().
   */
  g_object_set_qdata (object, quark_extension_type,
                      GSIZE_TO_POINTER (exten_type));

out:
  return object;
}
G_GNUC_END_IGNORE_DEPRECATIONS

static gboolean
peas_plugin_loader_gjs_load (PeasPluginLoader *loader,
                             PeasPluginInfo   *info)
{
  PeasPluginLoaderGjs *gjsloader = PEAS_PLUGIN_LOADER_GJS (loader);
  PeasPluginLoaderGjsData *loader_data;
  const char *module_dir, *module_name;
  char *module_dir_uri = NULL;
  char *module_name_js = NULL;
  char *module_uri = NULL;
  gboolean ret = FALSE;
  GError *error = NULL;

  module_name = peas_plugin_info_get_module_name (info);

  if ((loader_data = (PeasPluginLoaderGjsData *)g_hash_table_lookup (gjs_loader_info, module_name)))
    {
      info->loader_data = loader_data;
      return TRUE;
    }

  module_dir = peas_plugin_info_get_module_dir (info);
  module_name_js = g_strconcat (module_name, ".js", NULL);
  if (g_str_has_prefix (module_dir, "resource://"))
    module_dir_uri = g_strdup (module_dir);
  else
    module_dir_uri = g_strconcat ("file://", module_dir, NULL);
  module_uri = g_build_filename (module_dir_uri, module_name_js, NULL);

  if (!gjs_context_register_module (gjsloader->context, module_name, module_uri, &error))
    {
      g_debug ("Registering module %s threw exception: %s",
               module_uri, error->message);
      g_error_free (error);
      goto out;
    }

  loader_data = g_new0 (PeasPluginLoaderGjsData, 1);
  loader_data->loader = gjsloader;
  loader_data->gtypes = g_array_new (FALSE, FALSE, sizeof (GType));
  loader_data->module_name = g_intern_string (module_name);

  g_hash_table_insert (gjs_loader_info,
                       (gpointer)loader_data->module_name,
                       loader_data);

  gjs_context_run_in_realm (gjsloader->context, obtain_gtypes, loader_data);

  info->loader_data = loader_data;
  ret = TRUE;

out:
  g_free (module_name_js);
  g_free (module_dir_uri);
  g_free (module_uri);

  return ret;
}

static void
peas_plugin_loader_gjs_unload (PeasPluginLoader *loader,
                               PeasPluginInfo   *info)
{
  /* There is virtually no support for "unloading types" in GObject in a
   * realistic way as it would cause too much damage to objects and other
   * types in-flight.
   *
   * Therefore, we leak plugin loader data in a global hash table in
   * case the plugin is loaded again. Otherwise, GJS type registration
   * will complain about a type being loaded.
   *
   * We don't realistically support threading with the GJS loader anyway
   * as that would have to use JS workers or something similar to that.
   */
  info->loader_data = NULL;
}

static void
peas_plugin_loader_gjs_garbage_collect (PeasPluginLoader *loader)
{
  PeasPluginLoaderGjs *gjsloader = PEAS_PLUGIN_LOADER_GJS (loader);

  if (gjsloader->context != NULL)
    gjs_context_maybe_gc (gjsloader->context);
}

static gboolean
peas_plugin_loader_gjs_initialize (PeasPluginLoader *loader)
{
  PeasPluginLoaderGjs *gjsloader = PEAS_PLUGIN_LOADER_GJS (loader);
  GjsContext *context = gjs_context_get_current ();

  if (context != NULL)
    gjsloader->context = g_object_ref (context);
  else
    gjsloader->context = (GjsContext *)g_object_new (GJS_TYPE_CONTEXT, NULL);

  return TRUE;
}

static void
peas_plugin_loader_gjs_init (PeasPluginLoaderGjs *gjsloader)
{
}

static void
peas_plugin_loader_gjs_finalize (GObject *object)
{
  PeasPluginLoaderGjs *gjsloader = PEAS_PLUGIN_LOADER_GJS (object);

  g_clear_pointer (&gjs_loader_info, g_hash_table_unref);
  g_clear_object (&gjsloader->context);

  G_OBJECT_CLASS (peas_plugin_loader_gjs_parent_class)->finalize (object);
}

static void
peas_plugin_loader_gjs_data_free (gpointer data)
{
  PeasPluginLoaderGjsData *loader_data = (PeasPluginLoaderGjsData *)data;

  g_clear_pointer (&loader_data->gtypes, g_array_unref);
  g_free (loader_data);
}

static void
peas_plugin_loader_gjs_class_init (PeasPluginLoaderGjsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  PeasPluginLoaderClass *loader_class = PEAS_PLUGIN_LOADER_CLASS (klass);

  quark_extension_type = g_quark_from_static_string ("peas-extension-type");

  object_class->finalize = peas_plugin_loader_gjs_finalize;

  loader_class->initialize = peas_plugin_loader_gjs_initialize;
  loader_class->load = peas_plugin_loader_gjs_load;
  loader_class->unload = peas_plugin_loader_gjs_unload;
  loader_class->create_extension = peas_plugin_loader_gjs_create_extension;
  loader_class->provides_extension = peas_plugin_loader_gjs_provides_extension;
  loader_class->garbage_collect = peas_plugin_loader_gjs_garbage_collect;

  intern_plugin_info = g_intern_string ("plugin-info");

  gjs_loader_info = g_hash_table_new_full (g_str_hash,
                                           g_str_equal,
                                           NULL,
                                           peas_plugin_loader_gjs_data_free);
}
