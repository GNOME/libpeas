/*
 * peas-engine-priv.h
 * This file is part of libpeas
 *
 * Copyright (C) 2011 - Garrett Regier
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

#include <glib-object.h>

#include "peas-engine.h"
#include "peas-plugin-info.h"
#include "peas-version-macros.h"

G_BEGIN_DECLS

PEAS_AVAILABLE_IN_ALL
void     _peas_engine_shutdown          (void);
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
GObject *_peas_engine_create_extensionv (PeasEngine     *engine,
                                         PeasPluginInfo *info,
                                         GType           extension_type,
                                         guint           n_parameters,
                                         GParameter     *parameters);
G_GNUC_END_IGNORE_DEPRECATIONS

G_END_DECLS
