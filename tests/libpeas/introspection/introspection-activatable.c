/*
 * introspection-activatable.c
 * This file is part of libintrospection
 *
 * Copyright (C) 2010 Steve Frécinaux
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

#include "config.h"

#include "introspection-activatable.h"

/**
 * IntrospectionActivatable:
 *
 * Interface for activatable plugins.
 *
 * #IntrospectionActivatable is an interface which should be implemented by plugins
 * that should be activated on an object of a certain type (depending on the
 * application). For instance, in a typical windowed application,
 * #IntrospectionActivatable plugin instances could be bound to individual toplevel
 * windows.
 *
 * It is typical to use #IntrospectionActivatable along with [class@ExtensionSet] in order
 * to activate and deactivate extensions automatically when plugins are loaded
 * or unloaded.
 *
 * You can also use the code of this interface as a base for your own
 * extension types, as illustrated by gedit's %GeditWindowActivatable and
 * %GeditDocumentActivatable interfaces.
 **/

G_DEFINE_INTERFACE(IntrospectionActivatable, introspection_activatable, G_TYPE_OBJECT)

static void
introspection_activatable_default_init (IntrospectionActivatableInterface *iface)
{
  /**
   * IntrospectionActivatable:object:
   *
   * The object property contains the targetted object for this #IntrospectionActivatable
   * instance.
   *
   * For example a toplevel window in a typical windowed application. It is set
   * at construction time and won't change.
   */
  g_object_interface_install_property (iface,
                                       g_param_spec_object ("object",
                                                            "Object",
                                                            "Object",
                                                            G_TYPE_OBJECT,
                                                            G_PARAM_READWRITE |
                                                            G_PARAM_CONSTRUCT_ONLY |
                                                            G_PARAM_STATIC_STRINGS));
}

/**
 * introspection_activatable_activate:
 * @activatable: A #IntrospectionActivatable.
 *
 * Activates the extension on the targetted object.
 *
 * On activation, the extension should hook itself to the object
 * where it makes sense.
 */
void
introspection_activatable_activate (IntrospectionActivatable *activatable)
{
  IntrospectionActivatableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_ACTIVATABLE (activatable));

  iface = INTROSPECTION_ACTIVATABLE_GET_IFACE (activatable);
  g_return_if_fail (iface->activate != NULL);

  iface->activate (activatable);
}

/**
 * introspection_activatable_deactivate:
 * @activatable: A #IntrospectionActivatable.
 *
 * Deactivates the extension on the targetted object.
 *
 * On deactivation, an extension should remove itself from all the hooks it
 * used and should perform any cleanup required, so it can be unreffed safely
 * and without any more effect on the host application.
 */
void
introspection_activatable_deactivate (IntrospectionActivatable *activatable)
{
  IntrospectionActivatableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_ACTIVATABLE (activatable));

  iface = INTROSPECTION_ACTIVATABLE_GET_IFACE (activatable);
  g_return_if_fail (iface->deactivate != NULL);

  iface->deactivate (activatable);
}

/**
 * introspection_activatable_update_state:
 * @activatable: A #IntrospectionActivatable.
 *
 * Triggers an update of the extension internal state to take into account
 * state changes in the targetted object, due to some event or user action.
 */
void
introspection_activatable_update_state (IntrospectionActivatable *activatable)
{
  IntrospectionActivatableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_ACTIVATABLE (activatable));

  iface = INTROSPECTION_ACTIVATABLE_GET_IFACE (activatable);
  if (iface->update_state != NULL)
    iface->update_state (activatable);
}

