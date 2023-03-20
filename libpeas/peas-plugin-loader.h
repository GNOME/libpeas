/*
 * peas-plugin-loader.h
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

#include <glib-object.h>
#include <gmodule.h>

#include "peas-plugin-info.h"

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER (peas_plugin_loader_get_type())

PEAS_AVAILABLE_IN_ALL
G_DECLARE_DERIVABLE_TYPE (PeasPluginLoader, peas_plugin_loader, PEAS, PLUGIN_LOADER, GObject)

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
struct _PeasPluginLoaderClass {
  GObjectClass parent;

  gboolean  (*initialize)         (PeasPluginLoader *loader);
  gboolean  (*is_global)          (PeasPluginLoader *loader);
  gboolean  (*load)               (PeasPluginLoader *loader,
                                   PeasPluginInfo   *info);
  void      (*unload)             (PeasPluginLoader *loader,
                                   PeasPluginInfo   *info);
  gboolean  (*provides_extension) (PeasPluginLoader *loader,
                                   PeasPluginInfo   *info,
                                   GType             ext_type);
  GObject  *(*create_extension)   (PeasPluginLoader *loader,
                                   PeasPluginInfo   *info,
                                   GType             ext_type,
                                   guint             n_parameters,
                                   GParameter       *parameters);
  void      (*garbage_collect)    (PeasPluginLoader *loader);

  /*< private >*/
  gpointer _reserved[8];
};
G_GNUC_END_IGNORE_DEPRECATIONS

PEAS_AVAILABLE_IN_ALL
gboolean  peas_plugin_loader_initialize         (PeasPluginLoader *loader);
PEAS_AVAILABLE_IN_ALL
gboolean  peas_plugin_loader_is_global          (PeasPluginLoader *loader);
PEAS_AVAILABLE_IN_ALL
gboolean  peas_plugin_loader_load               (PeasPluginLoader *loader,
                                                 PeasPluginInfo   *info);
PEAS_AVAILABLE_IN_ALL
void      peas_plugin_loader_unload             (PeasPluginLoader *loader,
                                                 PeasPluginInfo   *info);
PEAS_AVAILABLE_IN_ALL
gboolean  peas_plugin_loader_provides_extension (PeasPluginLoader *loader,
                                                 PeasPluginInfo   *info,
                                                 GType             ext_type);
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
GObject  *peas_plugin_loader_create_extension   (PeasPluginLoader *loader,
                                                 PeasPluginInfo   *info,
                                                 GType             ext_type,
                                                 guint             n_parameters,
                                                 GParameter       *parameters);
G_GNUC_END_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
void      peas_plugin_loader_garbage_collect    (PeasPluginLoader *loader);

G_END_DECLS
