/*
 * peas-plugin-info.c
 * This file is part of libpeas
 *
 * Copyright (C) 2002-2005 - Paolo Maggi
 * Copyright (C) 2007 - Steve Frécinaux
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

#include "peas-i18n-priv.h"
#include "peas-plugin-info-priv.h"
#include "peas-utils.h"

#ifdef G_OS_WIN32
#define OS_HELP_KEY "Help-Windows"
#elif defined(OS_OSX)
#define OS_HELP_KEY "Help-MacOS-X"
#else
#define OS_HELP_KEY "Help-GNOME"
#endif

/**
 * PeasPluginInfo:
 *
 * A #PeasPluginInfo contains all the information available about a plugin.
 *
 * All this information comes from the related plugin info file, whose file
 * extension is ".plugin". Here is an example of such a plugin file, in the
 * [struct@GLib.KeyFile] format:
 *
 * ```toml
 * [Plugin]
 * Module=helloworld
 * Depends=foo;bar;baz
 * Loader=python
 * Name=Hello World
 * Description=Displays "Hello World"
 * Authors=Steve Frécinaux &lt;code@istique.net&gt;
 * Copyright=Copyright © 2009-10 Steve Frécinaux
 * Website=https://wiki.gnome.org/Projects/Libpeas
 * Help=https://gitlab.gnome.org/GNOME/libpeas
 * Hidden=false
 * ```
 **/

G_DEFINE_QUARK (peas-plugin-info-error, peas_plugin_info_error)

G_DEFINE_FINAL_TYPE (PeasPluginInfo, peas_plugin_info, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_AUTHORS,
  PROP_BUILTIN,
  PROP_COPYRIGHT,
  PROP_DEPENDENCIES,
  PROP_DESCRIPTION,
  PROP_HELP_URI,
  PROP_HIDDEN,
  PROP_ICON_NAME,
  PROP_LOADED,
  PROP_MODULE_DIR,
  PROP_MODULE_NAME,
  PROP_NAME,
  PROP_VERSION,
  PROP_WEBSITE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

static void
peas_plugin_info_finalize (GObject *object)
{
  PeasPluginInfo *info = PEAS_PLUGIN_INFO (object);

  g_clear_pointer (&info->filename, g_free);
  g_clear_pointer (&info->module_dir, g_free);
  g_clear_pointer (&info->data_dir, g_free);
  g_clear_pointer (&info->embedded, g_free);
  g_clear_pointer (&info->module_name, g_free);
  g_clear_pointer (&info->dependencies, g_strfreev);
  g_clear_pointer (&info->name, g_free);
  g_clear_pointer (&info->desc, g_free);
  g_clear_pointer (&info->icon_name, g_free);
  g_clear_pointer (&info->website, g_free);
  g_clear_pointer (&info->copyright, g_free);
  g_clear_pointer (&info->version, g_free);
  g_clear_pointer (&info->help_uri, g_free);
  g_clear_pointer (&info->authors, g_strfreev);
  g_clear_pointer (&info->schema_source, g_settings_schema_source_unref);
  g_clear_pointer (&info->external_data, g_hash_table_unref);
  g_clear_pointer (&info->error, g_error_free);
  g_clear_pointer (&info->resources, g_ptr_array_unref);

  G_OBJECT_CLASS (peas_plugin_info_parent_class)->finalize (object);
}

static void
peas_plugin_info_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  PeasPluginInfo *info = PEAS_PLUGIN_INFO (object);

  switch (prop_id)
    {
    case PROP_AUTHORS:
      g_value_set_boxed (value, peas_plugin_info_get_authors (info));
      break;

    case PROP_BUILTIN:
      g_value_set_boolean (value, peas_plugin_info_is_builtin (info));
      break;

    case PROP_COPYRIGHT:
      g_value_set_string (value, peas_plugin_info_get_copyright (info));
      break;

    case PROP_DEPENDENCIES:
      g_value_set_boxed (value, peas_plugin_info_get_dependencies (info));
      break;

    case PROP_DESCRIPTION:
      g_value_set_string (value, peas_plugin_info_get_description (info));
      break;

    case PROP_HELP_URI:
      g_value_set_string (value, peas_plugin_info_get_help_uri (info));
      break;

    case PROP_HIDDEN:
      g_value_set_boolean (value, peas_plugin_info_is_hidden (info));
      break;

    case PROP_ICON_NAME:
      g_value_set_string (value, peas_plugin_info_get_icon_name (info));
      break;

    case PROP_LOADED:
      g_value_set_boolean (value, peas_plugin_info_is_loaded (info));
      break;

    case PROP_MODULE_DIR:
      g_value_set_string (value, peas_plugin_info_get_module_dir (info));
      break;

    case PROP_MODULE_NAME:
      g_value_set_string (value, peas_plugin_info_get_module_name (info));
      break;

    case PROP_NAME:
      g_value_set_string (value, peas_plugin_info_get_name (info));
      break;

    case PROP_VERSION:
      g_value_set_string (value, peas_plugin_info_get_version (info));
      break;

    case PROP_WEBSITE:
      g_value_set_string (value, peas_plugin_info_get_website (info));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
peas_plugin_info_class_init (PeasPluginInfoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = peas_plugin_info_finalize;
  object_class->get_property = peas_plugin_info_get_property;

  properties[PROP_BUILTIN] =
    g_param_spec_boolean (I_("builtin"), NULL, NULL,
                          FALSE,
                          (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_LOADED] =
    g_param_spec_boolean (I_("loaded"), NULL, NULL,
                          FALSE,
                          (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_MODULE_NAME] =
    g_param_spec_string (I_("module-name"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_MODULE_DIR] =
    g_param_spec_string (I_("module-dir"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_NAME] =
    g_param_spec_string (I_("name"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_COPYRIGHT] =
    g_param_spec_string (I_("copyright"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_DESCRIPTION] =
    g_param_spec_string (I_("description"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_ICON_NAME] =
    g_param_spec_string (I_("icon-name"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_WEBSITE] =
    g_param_spec_string (I_("website"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_VERSION] =
    g_param_spec_string (I_("version"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_HIDDEN] =
    g_param_spec_boolean (I_("hidden"), NULL, NULL,
                          FALSE,
                          (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_HELP_URI] =
    g_param_spec_string (I_("help-uri"), NULL, NULL,
                         NULL,
                         (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_DEPENDENCIES] =
    g_param_spec_boxed (I_("dependencies"), NULL, NULL,
                        G_TYPE_STRV,
                        (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  properties[PROP_AUTHORS] =
    g_param_spec_boxed (I_("authors"), NULL, NULL,
                        G_TYPE_STRV,
                        (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
peas_plugin_info_init (PeasPluginInfo *info)
{
}

/*
 * _peas_plugin_info_new:
 * @filename: The filename where to read the plugin information.
 * @module_dir: The module directory.
 * @data_dir: The data directory.
 *
 * Creates a new #PeasPluginInfo from a file on the disk.
 *
 * Returns: a newly created #PeasPluginInfo.
 */
PeasPluginInfo *
_peas_plugin_info_new (const char *filename,
                       const char *module_dir,
                       const char *data_dir)
{
  gsize i;
  gboolean is_resource;
  char *loader = NULL;
  char **strv, **keys;
  PeasPluginInfo *info;
  GKeyFile *plugin_file;
  GBytes *bytes = NULL;
  GError *error = NULL;

  g_return_val_if_fail (filename != NULL, NULL);

  is_resource = g_str_has_prefix (filename, "resource://");

  info = g_object_new (PEAS_TYPE_PLUGIN_INFO, NULL);

  plugin_file = g_key_file_new ();

  if (is_resource)
    {
      bytes = g_resources_lookup_data (filename + strlen ("resource://"),
                                       G_RESOURCE_LOOKUP_FLAGS_NONE,
                                       &error);
    }
  else
    {
      char *content;
      gsize length;

      if (g_file_get_contents (filename, &content, &length, &error))
        bytes = g_bytes_new_take (content, length);
    }

  if (bytes == NULL ||
      !g_key_file_load_from_data (plugin_file,
                                  g_bytes_get_data (bytes, NULL),
                                  g_bytes_get_size (bytes),
                                  G_KEY_FILE_NONE, &error))
    {
      g_warning ("Bad plugin file '%s': %s", filename, error->message);
      g_error_free (error);
      goto error;
    }

  /* Get module name */
  info->module_name = g_key_file_get_string (plugin_file, "Plugin",
                                             "Module", NULL);
  if (info->module_name == NULL || *info->module_name == '\0')
    {
      g_warning ("Could not find 'Module' in '[Plugin]' section in '%s'",
                 filename);
      goto error;
    }

  /* Get Name */
  info->name = g_key_file_get_locale_string (plugin_file, "Plugin",
                                      "Name", NULL, NULL);
  if (info->name == NULL || *info->name == '\0')
    {
      g_warning ("Could not find 'Name' in '[Plugin]' section in '%s'",
                 filename);
      goto error;
    }

  /* Get the loader for this plugin */
  loader = g_key_file_get_string (plugin_file, "Plugin", "Loader", NULL);
  if (loader == NULL || *loader == '\0')
    {
      /* Default to the C loader */
      info->loader_id = PEAS_UTILS_C_LOADER_ID;
    }
  else
    {
      info->loader_id = peas_utils_get_loader_id (loader);

      if (info->loader_id == -1)
        {
          g_warning ("Unkown 'Loader' in '[Plugin]' section in '%s': %s",
                     filename, loader);
          goto error;
        }
    }

  /* Get Embedded */
  info->embedded = g_key_file_get_string (plugin_file, "Plugin",
                                          "Embedded", NULL);
  if (info->embedded != NULL)
    {
      if (info->loader_id != PEAS_UTILS_C_LOADER_ID)
        {
          g_warning ("Bad plugin file '%s': embedded plugins "
                     "must use the C plugin loader", filename);
          goto error;
        }

      if (!is_resource)
        {
          g_warning ("Bad plugin file '%s': embedded plugins "
                     "must be a resource", filename);
          goto error;
        }
    }
  else if (is_resource &&
           info->loader_id != PEAS_UTILS_GJS_LOADER_ID)
    {
      g_warning ("Bad plugin file '%s': resource plugins must be embedded",
                 filename);
      goto error;
    }

  /* Get the dependency list */
  info->dependencies = g_key_file_get_string_list (plugin_file,
                                                   "Plugin",
                                                   "Depends", NULL, NULL);
  if (info->dependencies == NULL)
    info->dependencies = g_new0 (char *, 1);

  /* Get Description */
  info->desc = g_key_file_get_locale_string (plugin_file, "Plugin",
                                             "Description", NULL, NULL);

  /* Get Icon */
  info->icon_name = g_key_file_get_locale_string (plugin_file, "Plugin",
                                                  "Icon", NULL, NULL);

  /* Get Authors */
  info->authors = g_key_file_get_string_list (plugin_file, "Plugin",
                                              "Authors", NULL, NULL);
  if (info->authors == NULL)
    info->authors = g_new0 (char *, 1);

  /* Get Copyright */
  strv = g_key_file_get_string_list (plugin_file, "Plugin",
                                     "Copyright", NULL, NULL);
  if (strv != NULL)
    {
      info->copyright = g_strjoinv ("\n", strv);

      g_strfreev (strv);
    }

  /* Get Website */
  info->website = g_key_file_get_string (plugin_file, "Plugin",
                                         "Website", NULL);

  /* Get Version */
  info->version = g_key_file_get_string (plugin_file, "Plugin",
                                         "Version", NULL);

  /* Get Help URI */
  info->help_uri = g_key_file_get_string (plugin_file, "Plugin",
                                          OS_HELP_KEY, NULL);
  if (info->help_uri == NULL)
    info->help_uri = g_key_file_get_string (plugin_file, "Plugin",
                                            "Help", NULL);

  /* Get Builtin */
  info->builtin = g_key_file_get_boolean (plugin_file, "Plugin",
                                          "Builtin", NULL);

  /* Get Hidden */
  info->hidden = g_key_file_get_boolean (plugin_file, "Plugin",
                                         "Hidden", NULL);

  keys = g_key_file_get_keys (plugin_file, "Plugin", NULL, NULL);

  for (i = 0; keys[i] != NULL; ++i)
    {
      if (!g_str_has_prefix (keys[i], "X-"))
        continue;

      if (info->external_data == NULL)
        info->external_data = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                     (GDestroyNotify) g_free,
                                                     (GDestroyNotify) g_free);

      g_hash_table_insert (info->external_data,
                           g_strdup (keys[i] + 2),
                           g_key_file_get_string (plugin_file, "Plugin",
                                                  keys[i], NULL));
    }

  g_strfreev (keys);

  g_free (loader);
  g_bytes_unref (bytes);
  g_key_file_free (plugin_file);

  info->filename = g_strdup (filename);
  info->module_dir = g_strdup (module_dir);
  info->data_dir = g_build_path (is_resource ? "/" : G_DIR_SEPARATOR_S,
                                 data_dir, info->module_name, NULL);

  /* If we know nothing about the availability of the plugin,
     set it as available */
  info->available = TRUE;

  return info;

error:

  g_object_unref (info);
  g_free (loader);
  g_clear_pointer (&bytes, g_bytes_unref);
  g_key_file_free (plugin_file);

  return NULL;
}

/**
 * peas_plugin_info_is_loaded:
 * @info: A #PeasPluginInfo.
 *
 * Check if the plugin is loaded.
 *
 * Returns: %TRUE if the plugin is loaded.
 */
gboolean
peas_plugin_info_is_loaded (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, FALSE);

  return info->available && info->loaded;
}

/**
 * peas_plugin_info_is_available:
 * @info: A #PeasPluginInfo.
 * @error: A #GError.
 *
 * Check if the plugin is available.
 *
 * A plugin is marked as not available when there is no loader available to
 * load it, or when there has been an error when trying to load it previously.
 * If not available then @error will be set.
 *
 * Returns: %TRUE if the plugin is available.
 */
gboolean
peas_plugin_info_is_available (const PeasPluginInfo  *info,
                               GError               **error)
{
  g_return_val_if_fail (info != NULL, FALSE);

  /* Uses g_propagate_error() so we get the right warning
   * in the case that *error != NULL
   */
  if (error != NULL && info->error != NULL)
    g_propagate_error (error, g_error_copy (info->error));

  return info->available != FALSE;
}

/**
 * peas_plugin_info_is_builtin:
 * @info: A #PeasPluginInfo.
 *
 * Check if the plugin is a builtin plugin.
 *
 * A builtin plugin is a plugin which cannot be enabled or disabled by the user
 * through a plugin manager (like
 * [PeasGtkPluginManager](https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-gtk-1.0/class.PluginManager.html)).
 * Loading or unloading such plugins is the responsibility of the application
 * alone. Most applications will usually load those plugins immediately after
 * the initialization of the #PeasEngine.
 *
 * The relevant key in the plugin info file is "Builtin".
 *
 * Returns: %TRUE if the plugin is a builtin plugin, %FALSE
 *   if not.
 **/
gboolean
peas_plugin_info_is_builtin (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, TRUE);

  return info->builtin;
}

/**
 * peas_plugin_info_is_hidden:
 * @info: A #PeasPluginInfo.
 *
 * Check if the plugin is a hidden plugin.
 *
 * A hidden plugin is a plugin which cannot be seen by a
 * user through a plugin manager (like
 * [PeasGtkPluginManager](https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-gtk-1.0/class.PluginManager.html)).
 * Loading and
 * unloading such plugins is the responsibility of the application alone or
 * through plugins that depend on them.
 *
 * The relevant key in the plugin info file is "Hidden".
 *
 * Returns: %TRUE if the plugin is a hidden plugin, %FALSE
 *   if not.
 **/
gboolean
peas_plugin_info_is_hidden (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, FALSE);

  return info->hidden;
}

/**
 * peas_plugin_info_get_module_name:
 * @info: A #PeasPluginInfo.
 *
 * Gets the module name.
 *
 * The module name will be used to find the actual plugin. The way this value
 * will be used depends on the loader (i.e. on the language) of the plugin.
 * This value is also used to uniquely identify a particular plugin.
 *
 * The relevant key in the plugin info file is "Module".
 *
 * Returns: the module name.
 */
const char *
peas_plugin_info_get_module_name (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->module_name;
}

/**
 * peas_plugin_info_get_module_dir:
 * @info: A #PeasPluginInfo.
 *
 * Gets the module directory.
 *
 * The module directory is the directory where the plugin file was found. This
 * is not a value from the [struct@GLib.KeyFile], but rather a value provided by the
 * [class@Engine].
 *
 * Returns: the module directory.
 */
const char *
peas_plugin_info_get_module_dir (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->module_dir;
}

/**
 * peas_plugin_info_get_data_dir:
 * @info: A #PeasPluginInfo.
 *
 * Gets the data dir of the plugin.
 *
 * The module data directory is the directory where a plugin should find its
 * runtime data. This is not a value read from the [struct@GLib.KeyFile], but
 * rather a value provided by the [class@Engine], depending on where the plugin
 * file was found.
 *
 * Returns: the plugin's data dir.
 */
const char *
peas_plugin_info_get_data_dir (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->data_dir;
}

/**
 * peas_plugin_info_get_settings:
 * @info: A #PeasPluginInfo.
 * @schema_id: (allow-none): The schema id.
 *
 * Creates a new [class@Gio.Settings] for the given @schema_id and if
 * gschemas.compiled is not in the module directory an attempt
 * will be made to create it.
 *
 * Returns: (transfer full) (nullable): a new #GSettings.
 *
 * Since: 1.4
 */
GSettings *
peas_plugin_info_get_settings (const PeasPluginInfo *info,
                               const char           *schema_id)
{
  GSettingsSchema *schema;
  GSettings *settings;

  g_return_val_if_fail (info != NULL, NULL);

  if (info->schema_source == NULL)
    {
      GFile *module_dir_location;
      GFile *gschema_compiled;
      GSettingsSchemaSource *default_source;

      module_dir_location = g_file_new_for_path (info->module_dir);
      gschema_compiled = g_file_get_child (module_dir_location,
                                           "gschemas.compiled");

      if (!g_file_query_exists (gschema_compiled, NULL))
        {
          const char *argv[] = {
            "glib-compile-schemas",
            "--targetdir", info->module_dir,
            info->module_dir,
            NULL
          };

          g_spawn_sync (NULL, (char **) argv, NULL, G_SPAWN_SEARCH_PATH,
                        NULL, NULL, NULL, NULL, NULL, NULL);
        }

      g_object_unref (gschema_compiled);
      g_object_unref (module_dir_location);

      default_source = g_settings_schema_source_get_default ();
      ((PeasPluginInfo *) info)->schema_source =
            g_settings_schema_source_new_from_directory (info->module_dir,
                                                         default_source,
                                                         FALSE, NULL);

      /* glib-compile-schemas already outputted a message */
      if (info->schema_source == NULL)
        return NULL;
    }

  if (schema_id == NULL)
    schema_id = info->module_name;

  schema = g_settings_schema_source_lookup (info->schema_source, schema_id,
                                            FALSE);

  if (schema == NULL)
    return NULL;

  settings = g_settings_new_full (schema, NULL, NULL);

  g_settings_schema_unref (schema);

  return settings;
}

/**
 * peas_plugin_info_get_dependencies:
 * @info: A #PeasPluginInfo.
 *
 * Gets the dependencies of the plugin.
 *
 * The [class@Engine] will always ensure that the dependencies of a plugin are
 * loaded when the said plugin is loaded. It means that dependencies are
 * loaded before the plugin, and unloaded after it. Circular dependencies of
 * plugins lead to undefined loading order.
 *
 * The relevant key in the plugin info file is "Depends".
 *
 * Returns: (transfer none): the plugin's dependencies.
 */
const char * const *
peas_plugin_info_get_dependencies (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return (const char * const *) info->dependencies;
}

/**
 * peas_plugin_info_has_dependency:
 * @info: A #PeasPluginInfo.
 * @module_name: The name of the plugin to check.
 *
 * Check if the plugin depends on another plugin.
 *
 * Returns: whether the plugin depends on the plugin @module_name.
 */
gboolean
peas_plugin_info_has_dependency (const PeasPluginInfo *info,
                                 const char           *module_name)
{
  guint i;

  g_return_val_if_fail (info != NULL, FALSE);
  g_return_val_if_fail (module_name != NULL, FALSE);

  for (i = 0; info->dependencies[i] != NULL; i++)
    {
      if (g_ascii_strcasecmp (module_name, info->dependencies[i]) == 0)
        return TRUE;
    }

  return FALSE;
}


/**
 * peas_plugin_info_get_name:
 * @info: A #PeasPluginInfo.
 *
 * Gets the name of the plugin.
 *
 * The name of a plugin should be a nice short string to be presented in UIs.
 *
 * The relevant key in the plugin info file is "Name".
 *
 * Returns: the plugin's name.
 */
const char *
peas_plugin_info_get_name (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->name;
}

/**
 * peas_plugin_info_get_description:
 * @info: A #PeasPluginInfo.
 *
 * Gets the description of the plugin.
 *
 * The description of the plugin should be a string presenting the purpose of
 * the plugin. It will typically be presented in a plugin's about box.
 *
 * The relevant key in the plugin info file is "Description".
 *
 * Returns: the plugin's description.
 */
const char *
peas_plugin_info_get_description (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->desc;
}

/**
 * peas_plugin_info_get_icon_name:
 * @info: A #PeasPluginInfo.
 *
 * Gets the icon name of the plugin.
 *
 * The icon of the plugin will be presented in the plugin manager UI. If no
 * icon is specified, the default green puzzle icon will be used.
 *
 * The relevant key in the plugin info file is "Icon".
 *
 * Returns: the plugin's icon name.
 */
const char *
peas_plugin_info_get_icon_name (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  if (info->icon_name != NULL)
    return info->icon_name;

  return "libpeas-plugin";
}

/**
 * peas_plugin_info_get_authors:
 * @info: A #PeasPluginInfo.
 *
 * Gets a %NULL-terminated array of strings with the authors of the plugin.
 *
 * The relevant key in the plugin info file is "Authors".
 *
 * Returns: (transfer none) (array zero-terminated=1): the plugin's author list.
 */
const char * const *
peas_plugin_info_get_authors (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, (const char **) NULL);

  return (const char * const *) info->authors;
}

/**
 * peas_plugin_info_get_website:
 * @info: A #PeasPluginInfo.
 *
 * Gets the website of the plugin.
 *
 * The relevant key in the plugin info file is "Website".
 *
 * Returns: the plugin's associated website.
 */
const char *
peas_plugin_info_get_website (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->website;
}

/**
 * peas_plugin_info_get_copyright:
 * @info: A #PeasPluginInfo.
 *
 * Gets the copyright of the plugin.
 *
 * The relevant key in the plugin info file is "Copyright".
 *
 * Returns: the plugin's copyright information.
 */
const char *
peas_plugin_info_get_copyright (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->copyright;
}

/**
 * peas_plugin_info_get_version:
 * @info: A #PeasPluginInfo.
 *
 * Gets the version of the plugin.
 *
 * The relevant key in the plugin info file is "Version".
 *
 * Returns: the plugin's version.
 */
const char *
peas_plugin_info_get_version (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->version;
}

/**
 * peas_plugin_info_get_help_uri:
 * @info: A #PeasPluginInfo.
 *
 * Gets the help URI of the plugin.
 *
 * The Help URI of a plugin will typically be presented by the plugin manager
 * as a "Help" button linking to the URI. It can either be a HTTP URL on some
 * website or a ghelp: URI if a Gnome help page is available for the plugin.
 *
 * The relevant key in the plugin info file is "Help". Other platform-specific
 * keys exist for platform-specific help files. Those are "Help-GNOME",
 * "Help-Windows" and "Help-MacOS-X".
 *
 * Returns: the plugin's help URI.
 */
const char *
peas_plugin_info_get_help_uri (const PeasPluginInfo *info)
{
  g_return_val_if_fail (info != NULL, NULL);

  return info->help_uri;
}

/**
 * peas_plugin_info_get_external_data:
 * @info: A #PeasPluginInfo.
 * @key: The key to lookup.
 *
 * Gets external data specified for the plugin.
 *
 * External data is specified in the plugin info file prefixed with X-. For
 * example, if a key/value pair `X-Peas=1` is specified in the key file, you
 * can use "Peas" for @key to retrieve the value "1".
 *
 * Note: that you can omit the X- prefix when retrieving the value,
 * but not when specifying the value in the file.
 *
 * Returns: (nullable): the external data, or %NULL if the external data could not be found.
 *
 * Since: 1.6
 */
const char *
peas_plugin_info_get_external_data (const PeasPluginInfo *info,
                                    const char           *key)
{
  g_return_val_if_fail (info != NULL, NULL);
  g_return_val_if_fail (key != NULL, NULL);

  if (info->external_data == NULL)
    return NULL;

  if (g_str_has_prefix (key, "X-"))
    key += 2;

  return g_hash_table_lookup (info->external_data, key);
}

 /**
 * peas_plugin_info_get_resource:
 * @info: A #PeasPluginInfo.
 * @filename: (allow-none): The filename of the resource, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GResource for the given @filename
 * located in the module directory. If @filename is %NULL
 * then "${module_name}.gresource" will be loaded.
 *
 * Returns: (transfer full): a new #GResource, or %NULL.
 */
GResource *
peas_plugin_info_get_resource (const PeasPluginInfo  *info,
                               const char           *filename,
                               GError               **error)
{
  char *default_filename = NULL;
  char *full_filename;
  GResource *resource;

  g_return_val_if_fail (info != NULL, NULL);
  g_return_val_if_fail (filename == NULL || filename[0] != '\0', NULL);
  g_return_val_if_fail (error == NULL || *error == NULL, NULL);

  if (filename == NULL)
    {
      default_filename = g_strconcat (info->module_name, ".gresource", NULL);
      filename = (const char *) default_filename;
    }

  full_filename = g_build_filename (info->module_dir, filename, NULL);
  resource = g_resource_load (full_filename, error);

  g_free (full_filename);
  g_free (default_filename);

  return resource;
}

/**
 * peas_plugin_info_load_resource:
 * @info: A #PeasPluginInfo.
 * @filename: (allow-none): The filename of the resource, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Loads the resource using peas_plugin_info_get_resource() and
 * registers it. The resource's lifetime will be automatically
 * handled by @info.
 *
 * See peas_plugin_info_get_resource() for more information.
 */
void
peas_plugin_info_load_resource (const PeasPluginInfo  *info,
                                const char            *filename,
                                GError               **error)
{
  GResource *resource;
  PeasPluginInfo *info_ = (PeasPluginInfo *) info;

  resource = peas_plugin_info_get_resource (info, filename, error);

  if (resource == NULL)
    return;

  if (info_->resources == NULL)
    {
      info_->resources = g_ptr_array_new ();
      g_ptr_array_set_free_func (info_->resources,
                                 (GDestroyNotify) g_resources_unregister);
    }

  g_resources_register (resource);

  g_ptr_array_add (info_->resources, resource);
  g_resource_unref (resource);
}
