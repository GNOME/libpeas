/*
 * extension-gjs.c
 * This file is part of libpeas
 *
 * Copyright 2011 - Steve Frécinaux
 * Copyright 2011-2013 - Garrett Regier
 * Copyright 2023 - Christian Hergert
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

#include "libpeas/peas-engine-priv.h"

#include "testing/testing-extension.h"

#include "introspection/introspection-activatable.h"
#include "introspection/introspection-base.h"

#define GJS_LOADER      gjs
#define GJS_LOADER_STR  G_STRINGIFY (GJS_LOADER)


static void
test_extension_gjs_instance_refcount (PeasEngine     *engine,
                                      PeasPluginInfo *info)
{
  GObject *extension;

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  g_assert_true (G_IS_OBJECT (extension));
  g_assert_true (INTROSPECTION_IS_BASE (extension));

  /* GJS will add_toggle_ref which increases the reference count by 1. */
  g_assert_cmpint (extension->ref_count, ==, 2);

  g_object_unref (extension);
}

static void
test_extension_gjs_activatable_subject_refcount (PeasEngine     *engine,
                                                 PeasPluginInfo *info)
{
  GObject *extension;
  GObject *object;

  /* Create the 'object' property value, to be similar to a GtkWindow
   * instance: a sunk GInitiallyUnowned object. */
  object = g_object_new (G_TYPE_INITIALLY_UNOWNED, NULL);
  g_object_ref_sink (object);
  g_assert_cmpint (object->ref_count, ==, 1);

  /* we pre-create the wrapper to make it easier to check reference count */
  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_ACTIVATABLE,
                                            "object", object,
                                            NULL);

  g_assert_true (G_IS_OBJECT (extension));

  g_assert_cmpint (object->ref_count, ==, 2);
  g_assert_cmpint (G_OBJECT (extension)->ref_count, ==, 2);

  /* TODO: It would be nice to check if GJS handled the toggle unref properly,
   * but that currently requires access to a periodic flush we can't control.
   */

  g_object_unref (extension);
  g_object_unref (object);
}

static void
test_extension_gjs_nonexistent (PeasEngine *engine)
{
  PeasPluginInfo *info;

  testing_util_push_log_hook ("Error loading plugin 'extension-" GJS_LOADER_STR "-nonexistent'");

  info = peas_engine_get_plugin_info (engine,
                                      "extension-" GJS_LOADER_STR "-nonexistent");

  g_assert_true (info != NULL);
  g_assert_true (!peas_engine_load_plugin (engine, info));
}

int
main (int   argc,
      char *argv[])
{
  testing_init (&argc, &argv);

  /* Only test the basics */
  testing_extension_basic (GJS_LOADER_STR);

#undef EXTENSION_TEST
#undef EXTENSION_TEST_FUNC

#define EXTENSION_TEST(loader, path, func) \
  testing_extension_add (EXTENSION_TEST_NAME (loader, path), \
                         (gpointer) test_extension_gjs_##func)

#define EXTENSION_TEST_FUNC(loader, path, func) \
  g_test_add_func (EXTENSION_TEST_NAME (loader, path), \
                   (gpointer) test_extension_gjs_##func)

  EXTENSION_TEST (GJS_LOADER, "instance-refcount", instance_refcount);
  EXTENSION_TEST (GJS_LOADER, "activatable-subject-refcount",
                  activatable_subject_refcount);

  EXTENSION_TEST (GJS_LOADER, "nonexistent", nonexistent);

  return testing_extension_run_tests ();
}
