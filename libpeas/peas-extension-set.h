/*
 * peas-extension-set.h
 * This file is part of libpeas
 *
 * Copyright (C) 2010 - Steve Frécinaux
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

#include "peas-engine.h"
#include "peas-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_EXTENSION_SET (peas_extension_set_get_type())

PEAS_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (PeasExtensionSet, peas_extension_set, PEAS, EXTENSION_SET, GObject)

/**
 * PeasExtensionSetForeachFunc:
 * @set: A #PeasExtensionSet.
 * @info: A #PeasPluginInfo.
 * @extension: The extension instance.
 * @data: Optional data passed to the function.
 *
 * This function is passed to [method@ExtensionSet.foreach] and
 * will be called for each extension in @set.
 */
typedef void (*PeasExtensionSetForeachFunc) (PeasExtensionSet *set,
                                             PeasPluginInfo   *info,
                                             GObject          *extension,
                                             gpointer          data);

PEAS_AVAILABLE_IN_ALL
void              peas_extension_set_foreach             (PeasExtensionSet             *set,
                                                          PeasExtensionSetForeachFunc   func,
                                                          gpointer                      data);
PEAS_AVAILABLE_IN_ALL
GObject          *peas_extension_set_get_extension       (PeasExtensionSet             *set,
                                                          PeasPluginInfo               *info);
PEAS_AVAILABLE_IN_ALL
PeasExtensionSet *peas_extension_set_new_with_properties (PeasEngine                   *engine,
                                                          GType                         exten_type,
                                                          guint                         n_properties,
                                                          const char                  **prop_names,
                                                          const GValue                 *prop_values);
PEAS_AVAILABLE_IN_ALL
PeasExtensionSet *peas_extension_set_new_valist          (PeasEngine                   *engine,
                                                          GType                         exten_type,
                                                          const char                   *first_property,
                                                          va_list                       var_args);
PEAS_AVAILABLE_IN_ALL
PeasExtensionSet *peas_extension_set_new                 (PeasEngine                   *engine,
                                                          GType                         exten_type,
                                                          const char                   *first_property,
                                                          ...);

G_END_DECLS
