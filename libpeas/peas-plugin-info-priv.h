/*
 * peas-plugin-info-priv.h
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

#include "peas-plugin-info.h"

struct _PeasPluginInfo {
  GObject parent_instance;

  /*< private >*/

  /* Used and managed by PeasPluginLoader */
  gpointer loader_data;

  char *filename;
  char *module_dir;
  char *data_dir;

  int loader_id;
  char *embedded;
  char *module_name;
  char **dependencies;

  char *name;
  char *desc;
  char *icon_name;
  char **authors;
  char *copyright;
  char *website;
  char *version;
  char *help_uri;

  GHashTable *external_data;

  GPtrArray *resources;
  GSettingsSchemaSource *schema_source;

  GError *error;

  guint loaded : 1;
  /* A plugin is unavailable if it is not possible to load it
     due to an error loading the plugin module (e.g. for Python plugins
     when the interpreter has not been correctly initializated) */
  guint available : 1;

  guint builtin : 1;
  guint hidden : 1;
};

PeasPluginInfo *_peas_plugin_info_new (const char *filename,
                                       const char *module_dir,
                                       const char *data_dir);
