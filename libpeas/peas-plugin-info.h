/*
 * peas-plugins-info.h
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

#pragma once

#if !defined (LIBPEAS_INSIDE) && !defined (LIBPEAS_COMPILATION)
# error "Only <libpeas.h> can be included directly."
#endif

#include <gio/gio.h>

#include "peas-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_INFO (peas_plugin_info_get_type())

/**
 * PEAS_PLUGIN_INFO_ERROR:
 *
 * Error domain for PeasPluginInfo. Errors in this domain will
 * be from the PeasPluginInfoError enumeration. See #GError for
 * more information on error domains.
 */
#define PEAS_PLUGIN_INFO_ERROR peas_plugin_info_error_quark ()

/**
 * PeasPluginInfoError:
 * @PEAS_PLUGIN_INFO_ERROR_LOADING_FAILED:
 *      The plugin failed to load.
 * @PEAS_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND:
 *      The plugin's loader was not found.
 * @PEAS_PLUGIN_INFO_ERROR_DEP_NOT_FOUND:
 *      A dependency of the plugin was not found.
 * @PEAS_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED:
 *      A dependency of the plugin failed to load.
 *
 * These identify the various errors that can occur while
 * loading a plugin.
 */
typedef enum {
  PEAS_PLUGIN_INFO_ERROR_LOADING_FAILED,
  PEAS_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND,
  PEAS_PLUGIN_INFO_ERROR_DEP_NOT_FOUND,
  PEAS_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED
} PeasPluginInfoError;

PEAS_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (PeasPluginInfo, peas_plugin_info, PEAS, PLUGIN_INFO, GObject)

PEAS_AVAILABLE_IN_ALL
GQuark               peas_plugin_info_error_quark       (void);
PEAS_AVAILABLE_IN_ALL
gboolean             peas_plugin_info_is_loaded         (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean             peas_plugin_info_is_available      (const PeasPluginInfo  *info,
                                                         GError               **error);
PEAS_AVAILABLE_IN_ALL
gboolean             peas_plugin_info_is_builtin        (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean             peas_plugin_info_is_hidden         (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_module_name   (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_module_dir    (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_data_dir      (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
GSettings           *peas_plugin_info_get_settings      (const PeasPluginInfo  *info,
                                                         const char            *schema_id);
PEAS_AVAILABLE_IN_ALL
const char * const  *peas_plugin_info_get_dependencies  (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean             peas_plugin_info_has_dependency    (const PeasPluginInfo  *info,
                                                         const char            *module_name);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_name          (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_description   (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_icon_name     (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char * const  *peas_plugin_info_get_authors       (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_website       (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_copyright     (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_version       (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_help_uri      (const PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
const char          *peas_plugin_info_get_external_data (const PeasPluginInfo  *info,
                                                         const char            *key);
PEAS_AVAILABLE_IN_ALL
GResource           *peas_plugin_info_get_resource      (const PeasPluginInfo  *info,
                                                         const char            *filename,
                                                         GError               **error);
PEAS_AVAILABLE_IN_ALL
void                 peas_plugin_info_load_resource     (const PeasPluginInfo  *info,
                                                         const char            *filename,
                                                         GError               **error);

G_END_DECLS
