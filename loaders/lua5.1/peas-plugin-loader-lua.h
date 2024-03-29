/*
 * peas-plugin-loader-lua.h
 * This file is part of libpeas
 *
 * Copyright (C) 2014 - Garrett Regier
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

#include <libpeas/peas-plugin-loader.h>
#include <libpeas/peas-object-module.h>

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER_LUA             (peas_plugin_loader_lua_get_type ())
#define PEAS_PLUGIN_LOADER_LUA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_PLUGIN_LOADER_LUA, PeasPluginLoaderLua))
#define PEAS_PLUGIN_LOADER_LUA_CONST(obj)       (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_PLUGIN_LOADER_LUA, PeasPluginLoaderLua const))
#define PEAS_PLUGIN_LOADER_LUA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_TYPE_PLUGIN_LOADER_LUA, PeasPluginLoaderLuaClass))
#define PEAS_IS_PLUGIN_LOADER_LUA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_PLUGIN_LOADER_LUA))
#define PEAS_IS_PLUGIN_LOADER_LUA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_PLUGIN_LOADER_LUA))
#define PEAS_PLUGIN_LOADER_LUA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PEAS_TYPE_PLUGIN_LOADER_LUA, PeasPluginLoaderLuaClass))

typedef struct _PeasPluginLoaderLua         PeasPluginLoaderLua;
typedef struct _PeasPluginLoaderLuaClass    PeasPluginLoaderLuaClass;

struct _PeasPluginLoaderLua {
  PeasPluginLoader parent;
};

struct _PeasPluginLoaderLuaClass {
  PeasPluginLoaderClass parent_class;
};

GType                    peas_plugin_loader_lua_get_type  (void) G_GNUC_CONST;

/* All the loaders must implement this function */
G_MODULE_EXPORT void     peas_register_types              (PeasObjectModule *module);

G_END_DECLS
