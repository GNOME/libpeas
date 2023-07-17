/*
 * extension-set.c
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

#include "config.h"

#include <stdlib.h>

#include <glib.h>
#include <glib-object.h>
#include <libpeas.h>

#include "introspection-activatable.h"

#include "testing/testing.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  PeasEngine *engine;
};

/* Have dependencies before the plugin that requires them */
static const char *loadable_plugins[] = {
  "loadable", "has-dep", "self-dep"
};

static void
extension_added_cb (PeasExtensionSet *extension_set,
                    PeasPluginInfo   *info,
                    GObject    *extension,
                    int             *active)
{
  ++(*active);
}

static void
extension_removed_cb (PeasExtensionSet *extension_set,
                      PeasPluginInfo   *info,
                      GObject    *extension,
                      int             *active)
{
  --(*active);
}

static PeasExtensionSet *
testing_extension_set_new (PeasEngine *engine,
                           int       *active)
{
  int i;
  PeasPluginInfo *info;
  PeasExtensionSet *extension_set;

  extension_set = peas_extension_set_new (engine,
                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                          "object", NULL,
                                          NULL);

  if (active == NULL)
    {
      active = g_new (int, 1);
      g_object_set_data_full (G_OBJECT (extension_set),
                              "testing-extension-set-active", active,
                              g_free);
    }

  *active = 0;

  g_signal_connect (extension_set,
                    "extension-added",
                    G_CALLBACK (extension_added_cb),
                    active);
  g_signal_connect (extension_set,
                    "extension-removed",
                    G_CALLBACK (extension_removed_cb),
                    active);

  peas_extension_set_foreach (extension_set,
                              (PeasExtensionSetForeachFunc) extension_added_cb,
                              active);

  for (i = 0; i < G_N_ELEMENTS (loadable_plugins); ++i)
    {
      g_assert_cmpint (*active, ==, i);

      info = peas_engine_get_plugin_info (engine, loadable_plugins[i]);
      g_assert_true (peas_engine_load_plugin (engine, info));
    }

  /* Load a plugin that does not provide a IntrospectionActivatable */
  info = peas_engine_get_plugin_info (engine, "extension-c");
  g_assert_true (peas_engine_load_plugin (engine, info));

  g_assert_cmpint (*active, ==, G_N_ELEMENTS (loadable_plugins));

  return extension_set;
}

static void
test_setup (TestFixture   *fixture,
            gconstpointer  data)
{
  fixture->engine = testing_engine_new ();
}

static void
test_teardown (TestFixture   *fixture,
               gconstpointer  data)
{
  testing_engine_free (fixture->engine);
}

static void
test_runner (TestFixture   *fixture,
             gconstpointer  data)
{
  ((void (*) (PeasEngine *engine)) data) (fixture->engine);
}

static void
test_extension_set_create_valid (PeasEngine *engine)
{
  PeasExtensionSet *extension_set;

  extension_set = peas_extension_set_new (engine,
                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                          "object", NULL,
                                          NULL);

  g_object_unref (extension_set);
}

static void
valid_extension_added_cb (PeasExtensionSet *extension_set,
                          PeasPluginInfo   *info,
                          GObject    *extension,
                          GObject          **obj_ptr)
{
  g_clear_object (obj_ptr);
  g_object_get (INTROSPECTION_ACTIVATABLE (extension), "object", obj_ptr, NULL);
}

static void
test_extension_set_create_valid_with_properties (PeasEngine *engine)
{
  PeasPluginInfo *info;
  PeasExtensionSet *extension_set;
  GValue prop_value = G_VALUE_INIT;
  GObject *obj;
  GObject *obj_cmp = NULL;
  const char *prop_names[1] = { "object" };

  obj = g_object_new (G_TYPE_OBJECT, NULL);
  g_value_init (&prop_value, G_TYPE_OBJECT);
  g_value_set_object (&prop_value, obj);

  extension_set = peas_extension_set_new_with_properties (engine,
                                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                                          G_N_ELEMENTS (prop_names),
                                                          prop_names,
                                                          &prop_value);
  g_signal_connect (extension_set,
                    "extension-added",
                    G_CALLBACK (valid_extension_added_cb),
                    &obj_cmp);
  info = peas_engine_get_plugin_info (engine, "builtin");

  g_assert_true (peas_engine_load_plugin (engine, info));
  g_assert_true (obj == obj_cmp);

  g_assert_true (PEAS_IS_EXTENSION_SET (extension_set));
  g_assert_finalize_object (extension_set);
  g_value_unset (&prop_value);

  g_object_unref (obj_cmp);
  g_assert_finalize_object (obj);
}


static void
test_extension_set_create_invalid (PeasEngine *engine)
{
  PeasExtensionSet *extension_set;

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*type 'IntrospectionActivatable' has no property named 'invalid-property'");

  /* Invalid GType */
  extension_set = peas_extension_set_new (engine, G_TYPE_INVALID, NULL);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));


  /* GObject but not a GInterface */
  extension_set = peas_extension_set_new (engine, G_TYPE_OBJECT, NULL);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));


  /* Interface does not have an 'invalid-property' property */
  extension_set = peas_extension_set_new (engine,
                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                          "invalid-property", "does-not-exist",
                                          NULL);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));
}

static void
test_extension_set_create_invalid_with_properties (PeasEngine *engine)
{
  PeasExtensionSet *extension_set;
  GValue prop_values[2] = { G_VALUE_INIT };
  const char *prop_names[2] = { "object", NULL };
  const char *prop_names_not_exist[1] = { "aleb" };
  guint n_elements;

  testing_util_push_log_hook ("*property name*should not be NULL.");
  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*should be an initialized GValue.");
  testing_util_push_log_hook ("*has no property named 'aleb'*");

  g_value_init (&prop_values[0], G_TYPE_POINTER);
  g_value_set_pointer (&prop_values[0], NULL);

  /* Interface has a NULL property name*/
  n_elements = G_N_ELEMENTS (prop_values);
  extension_set = peas_extension_set_new_with_properties (engine,
                                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                                          n_elements,
                                                          prop_names,
                                                          prop_values);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));
  g_value_unset (&prop_values[0]);

  /* Invalid GType */
  extension_set = peas_extension_set_new_with_properties (engine,
                                                          G_TYPE_INVALID,
                                                          0, NULL, NULL);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));

  /* Uninitialized GValue */
  n_elements = 1;
  extension_set = peas_extension_set_new_with_properties (engine,
                                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                                          n_elements,
                                                          prop_names,
                                                          prop_values);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));

  /* Uninitialized GValue*/
  g_value_init (&prop_values[0], G_TYPE_POINTER);
  g_value_set_pointer (&prop_values[0], NULL);
  n_elements = G_N_ELEMENTS (prop_names_not_exist);
  extension_set = peas_extension_set_new_with_properties (engine,
                                                          INTROSPECTION_TYPE_ACTIVATABLE,
                                                          n_elements,
                                                          prop_names_not_exist,
                                                          prop_values);
  g_assert_true (!PEAS_IS_EXTENSION_SET (extension_set));
  g_value_unset (&prop_values[0]);
}


static void
test_extension_set_extension_added (PeasEngine *engine)
{
  int active;
  PeasExtensionSet *extension_set;

  /* This will check that an extension is added
   * as plugins are loaded and cause active to
   * be synced with the number of added extensions
   */
  extension_set = testing_extension_set_new (engine, &active);

  g_object_unref (extension_set);

  /* Verify that freeing the extension
   * set causes the extensions to be removed
   */
  g_assert_cmpint (active, ==, 0);
}

static void
test_extension_set_extension_removed (PeasEngine *engine)
{
  int i, active;
  PeasPluginInfo *info;
  PeasExtensionSet *extension_set;

  extension_set = testing_extension_set_new (engine, &active);

  /* Unload the plugin that does not provide a IntrospectionActivatable */
  info = peas_engine_get_plugin_info (engine, "extension-c");
  g_assert_true (peas_engine_unload_plugin (engine, info));

  /* To keep deps in order */
  for (i = G_N_ELEMENTS (loadable_plugins); i > 0; --i)
    {
      g_assert_cmpint (active, ==, i);

      info = peas_engine_get_plugin_info (engine, loadable_plugins[i - 1]);

      g_assert_true (peas_engine_unload_plugin (engine, info));
    }

  g_assert_cmpint (active, ==, 0);

  g_object_unref (extension_set);
}

static void
test_extension_set_get_extension (PeasEngine *engine)
{
  PeasPluginInfo *info;
  GObject *extension;
  PeasExtensionSet *extension_set;

  extension_set = testing_extension_set_new (engine, NULL);
  info = peas_engine_get_plugin_info (engine, loadable_plugins[0]);

  extension = peas_extension_set_get_extension (extension_set, info);
  g_assert_true (INTROSPECTION_IS_ACTIVATABLE (extension));

  g_object_add_weak_pointer (G_OBJECT (extension),
                             (gpointer) &extension);
  g_assert_true (peas_engine_unload_plugin (engine, info));

  g_assert_true (extension == NULL);
  g_assert_true (peas_extension_set_get_extension (extension_set, info) == NULL);

  g_object_unref (extension_set);
}

static void
test_extension_set_foreach (PeasEngine *engine)
{
  int count = 0;
  PeasExtensionSet *extension_set;

  extension_set = testing_extension_set_new (engine, NULL);

  peas_extension_set_foreach (extension_set,
                              (PeasExtensionSetForeachFunc) extension_added_cb,
                              &count);

  g_assert_cmpint (count, ==, G_N_ELEMENTS (loadable_plugins));

  g_object_unref (extension_set);
}

static void
ordering_cb (PeasExtensionSet  *set,
             PeasPluginInfo    *info,
             GObject           *extension,
             GList            **order)
{
  const char *order_module_name = (const char *) (*order)->data;

  g_assert_cmpstr (order_module_name, ==,
                   peas_plugin_info_get_module_name (info));
  *order = g_list_delete_link (*order, *order);
}

static void
test_extension_set_ordering (PeasEngine *engine)
{
  guint i;
  GList *foreach_order = NULL;
  GList *dispose_order = NULL;
  PeasExtensionSet *extension_set;

  for (i = 0; i < G_N_ELEMENTS (loadable_plugins); ++i)
    {
      /* Use descriptive names to make an assert more informative */
      foreach_order = g_list_append (foreach_order,
                                     (gpointer) loadable_plugins[i]);
      dispose_order = g_list_prepend (dispose_order,
                                      (gpointer) loadable_plugins[i]);
    }

  extension_set = testing_extension_set_new (engine, NULL);

  peas_extension_set_foreach (extension_set,
                              (PeasExtensionSetForeachFunc) ordering_cb,
                              &foreach_order);
  g_assert_true (foreach_order == NULL);


  g_signal_connect (extension_set,
                    "extension-removed",
                    G_CALLBACK (ordering_cb),
                    &dispose_order);
  g_object_unref (extension_set);
  g_assert_true (dispose_order == NULL);
}

int
main (int    argc,
      char **argv)
{
  testing_init (&argc, &argv);

#define TEST(path, ftest) \
  g_test_add ("/extension-set/" path, TestFixture, \
              (gpointer) test_extension_set_##ftest, \
              test_setup, test_runner, test_teardown)

  TEST ("create-valid", create_valid);
  TEST ("create-invalid", create_invalid);
  TEST ("create-valid-with-properties", create_valid_with_properties);
  TEST ("create-invalid-with-properties", create_invalid_with_properties);

  TEST ("extension-added", extension_added);
  TEST ("extension-removed", extension_removed);

  TEST ("get-extension", get_extension);

  TEST ("foreach", foreach);

  TEST ("ordering", ordering);

#undef TEST

  return testing_run_tests ();
}
