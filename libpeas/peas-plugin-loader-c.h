/*
 * peas-plugin-loader-c.h
 * This file is part of libpeas
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#include "peas-plugin-loader.h"

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER_C (peas_plugin_loader_c_get_type())

G_DECLARE_FINAL_TYPE (PeasPluginLoaderC, peas_plugin_loader_c, PEAS, PLUGIN_LOADER_C, PeasPluginLoader)

PeasPluginLoader *peas_plugin_loader_c_new (void);

G_END_DECLS
