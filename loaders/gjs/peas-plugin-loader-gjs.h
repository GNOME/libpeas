/*
 * peas-plugin-loader-gjs.h
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

#pragma once

#include <libpeas.h>

#include "libpeas/peas-plugin-loader.h"

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER_GJS (peas_plugin_loader_gjs_get_type())

G_DECLARE_FINAL_TYPE (PeasPluginLoaderGjs, peas_plugin_loader_gjs, PEAS, PLUGIN_LOADER_GJS, PeasPluginLoader)

G_MODULE_EXPORT void peas_register_types (PeasObjectModule *module);

G_END_DECLS
