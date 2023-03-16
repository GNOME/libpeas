/*
 * introspection-unimplemented.h
 * This file is part of libpeas
 *
 * Copyright (C) 2010 - Garrett Regier
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

#include "../../testing-util/peas-test-common.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define INTROSPECTION_TYPE_UNIMPLEMENTED             (introspection_unimplemented_get_type ())
#define INTROSPECTION_UNIMPLEMENTED(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), INTROSPECTION_TYPE_UNIMPLEMENTED, IntrospectionUnimplemented))
#define INTROSPECTION_UNIMPLEMENTED_IFACE(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), INTROSPECTION_TYPE_UNIMPLEMENTED, IntrospectionUnimplementedInterface))
#define INTROSPECTION_IS_UNIMPLEMENTED(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), INTROSPECTION_TYPE_UNIMPLEMENTED))
#define INTROSPECTION_UNIMPLEMENTED_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), INTROSPECTION_TYPE_UNIMPLEMENTED, IntrospectionUnimplementedInterface))

typedef struct _IntrospectionUnimplemented           IntrospectionUnimplemented; /* dummy typedef */
typedef struct _IntrospectionUnimplementedInterface  IntrospectionUnimplementedInterface;

struct _IntrospectionUnimplementedInterface {
  GTypeInterface g_iface;
};

/*
 * Public methods
 */
PEAS_TEST_EXPORT
GType introspection_unimplemented_get_type (void)  G_GNUC_CONST;

G_END_DECLS
