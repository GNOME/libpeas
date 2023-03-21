/*
 * peas-engine.h
 * This file is part of libpeas
 *
 * Copyright (C) 2002-2005 - Paolo Maggi
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

#include <glib-object.h>

#include "peas-plugin-info.h"
#include "peas-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_ENGINE (peas_engine_get_type())

PEAS_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (PeasEngine, peas_engine, PEAS, ENGINE, GObject)

PEAS_AVAILABLE_IN_ALL
PeasEngine      *peas_engine_new                              (void);
PEAS_AVAILABLE_IN_ALL
PeasEngine      *peas_engine_new_with_nonglobal_loaders       (void);
PEAS_AVAILABLE_IN_ALL
PeasEngine      *peas_engine_get_default                      (void);
PEAS_AVAILABLE_IN_ALL
void             peas_engine_add_search_path                  (PeasEngine      *engine,
                                                               const char      *module_dir,
                                                               const char      *data_dir);
PEAS_AVAILABLE_IN_ALL
void             peas_engine_enable_loader                    (PeasEngine      *engine,
                                                               const char      *loader_name);
PEAS_AVAILABLE_IN_ALL
void             peas_engine_rescan_plugins                   (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
char           **peas_engine_dup_loaded_plugins               (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
void             peas_engine_set_loaded_plugins               (PeasEngine      *engine,
                                                               const char     **plugin_names);
PEAS_AVAILABLE_IN_ALL
PeasPluginInfo  *peas_engine_get_plugin_info                  (PeasEngine      *engine,
                                                               const char      *plugin_name);
PEAS_AVAILABLE_IN_ALL
gboolean         peas_engine_load_plugin                      (PeasEngine      *engine,
                                                               PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean         peas_engine_unload_plugin                    (PeasEngine      *engine,
                                                               PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
void             peas_engine_garbage_collect                  (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
gboolean         peas_engine_provides_extension               (PeasEngine      *engine,
                                                               PeasPluginInfo  *info,
                                                               GType            extension_type);
PEAS_AVAILABLE_IN_ALL
GObject         *peas_engine_create_extension_with_properties (PeasEngine      *engine,
                                                               PeasPluginInfo  *info,
                                                               GType            extension_type,
                                                               guint            n_properties,
                                                               const char     **prop_names,
                                                               const GValue    *prop_values);
PEAS_AVAILABLE_IN_ALL
GObject         *peas_engine_create_extension_valist          (PeasEngine      *engine,
                                                               PeasPluginInfo  *info,
                                                               GType            extension_type,
                                                               const char      *first_property,
                                                               va_list          var_args);
PEAS_AVAILABLE_IN_ALL
GObject         *peas_engine_create_extension                 (PeasEngine      *engine,
                                                               PeasPluginInfo  *info,
                                                               GType            extension_type,
                                                               const char      *first_property,
                                                               ...);

G_END_DECLS
