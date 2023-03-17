/*
 * peas-extension.c
 * This file is part of libpeas
 *
 * Copyright (C) 2010 Steve Frécinaux
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

#include "config.h"

#include "peas-extension.h"

/**
 * PeasExtension:
 *
 * #PeasExtension is a proxy class used to access actual extensions implemented
 * using various languages.
 *
 * As such, the application writer will use #PeasExtension instances to call
 * methods on extension provided by loaded plugins.
 *
 * To properly use the proxy instances, you will need GObject-introspection
 * data for the [alias@GObject.Type] you want to use as an extension point.
 * For instance, if you wish to use [iface@Activatable], you will need to
 * put the following code excerpt in the engine initialization code, in order
 * to load the required "Peas" typelib:
 *
 * ```c
 * g_irepository_require (g_irepository_get_default (),
 *                        "Peas", "1.0", 0, NULL);
 * ```
 *
 * You should proceed the same way for any namespace which provides types
 * you want to use as extension points. GObject-introspection data is required
 * for all the supported languages, even for C.
 *
 * #PeasExtension does not provide any way to access the underlying object.
 * The main reason is that some loaders may not rely on proper GObject
 * inheritance for the definition of extensions, and hence it would not be
 * possible for libpeas to provide a functional GObject instance at all.
 * Another reason is that it makes reference counting issues easier to deal
 * with.
 */
GType
peas_extension_get_type (void)
{
  return G_TYPE_OBJECT;
}

static G_DEFINE_QUARK (peas-extension-type, extension_type)

/**
 * peas_extension_get_extension_type:
 * @exten: A #PeasExtension.
 *
 * Get the [alias@GObject.Type] of the extension proxied by @exten.
 *
 * Returns: The #GType proxied by @exten.
 *
 * Deprecated: 1.2.
 */
GType
peas_extension_get_extension_type (PeasExtension *exten)
{
  return GPOINTER_TO_SIZE (g_object_get_qdata (G_OBJECT (exten),
                                               extension_type_quark ()));
}
