/*
 * introspection-activatable.h
 * This file is part of libintrospection
 *
 * Copyright (C) 2010 - Steve Frécinaux
 *
 * libintrospection is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libintrospection is distributed in the hope that it will be useful,
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

#define INTROSPECTION_TYPE_ACTIVATABLE             (introspection_activatable_get_type ())
#define INTROSPECTION_ACTIVATABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), INTROSPECTION_TYPE_ACTIVATABLE, IntrospectionActivatable))
#define INTROSPECTION_ACTIVATABLE_IFACE(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), INTROSPECTION_TYPE_ACTIVATABLE, IntrospectionActivatableInterface))
#define INTROSPECTION_IS_ACTIVATABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), INTROSPECTION_TYPE_ACTIVATABLE))
#define INTROSPECTION_ACTIVATABLE_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), INTROSPECTION_TYPE_ACTIVATABLE, IntrospectionActivatableInterface))

typedef struct _IntrospectionActivatable           IntrospectionActivatable; /* dummy typedef */
typedef struct _IntrospectionActivatableInterface  IntrospectionActivatableInterface;

struct _IntrospectionActivatableInterface {
  GTypeInterface g_iface;
  void        (*activate)                 (IntrospectionActivatable *activatable);
  void        (*deactivate)               (IntrospectionActivatable *activatable);
  void        (*update_state)             (IntrospectionActivatable *activatable);
};

PEAS_TEST_EXPORT
GType             introspection_activatable_get_type       (void)  G_GNUC_CONST;
PEAS_TEST_EXPORT
void              introspection_activatable_activate       (IntrospectionActivatable *activatable);
PEAS_TEST_EXPORT
void              introspection_activatable_deactivate     (IntrospectionActivatable *activatable);
PEAS_TEST_EXPORT
void              introspection_activatable_update_state   (IntrospectionActivatable *activatable);

G_END_DECLS
