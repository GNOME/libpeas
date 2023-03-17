/*
 * peas-extension-base.h
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

#define PEAS_TYPE_EXTENSION_BASE (peas_extension_base_get_type())

PEAS_AVAILABLE_IN_ALL
G_DECLARE_DERIVABLE_TYPE (PeasExtensionBase, peas_extension_base, PEAS, EXTENSION_BASE, GObject)

/**
 * PeasExtensionBaseClass:
 * @parent_class: The parent class.
 *
 * The class structure of #PeasExtensionBase.
 */
struct _PeasExtensionBaseClass
{
  GObjectClass parent_class;

  /*< private >*/
  gpointer _reserved[8];
};

PEAS_AVAILABLE_IN_ALL
PeasPluginInfo *peas_extension_base_get_plugin_info (PeasExtensionBase *extbase);
PEAS_AVAILABLE_IN_ALL
char           *peas_extension_base_get_data_dir    (PeasExtensionBase *extbase);

G_END_DECLS
