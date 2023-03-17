/*
 * peas-extension.h
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

#include "peas-version-macros.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define PEAS_TYPE_EXTENSION    (G_TYPE_OBJECT)
#define PEAS_EXTENSION(obj)    (G_OBJECT(obj))
#define PEAS_IS_EXTENSION(obj) (G_IS_OBJECT(obj))

/**
 * PeasExtension:
 *
 * A proxy class to access the actual plugin.
 */
typedef GObject PeasExtension;

G_END_DECLS
