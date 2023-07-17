/*
 * testing-extension.c
 * This file is part of libpeas
 *
 * Copyright (C) 2011-2014 - Garrett Regier
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
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <libpeas.h>

#include "testing.h"
#include "testing-extension.h"

#include "introspection-abstract.h"
#include "introspection-activatable.h"
#include "introspection-base.h"
#include "introspection-callable.h"
#include "introspection-has-prerequisite.h"
#include "introspection-unimplemented.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  PeasEngine *engine;
  PeasPluginInfo *info;
};

static char *loader = NULL;
static char *extension_plugin = NULL;

static void
test_setup (TestFixture    *fixture,
            gconstpointer  data)
{
  fixture->engine = testing_engine_new ();
  peas_engine_enable_loader (fixture->engine, loader);
  fixture->info = peas_engine_get_plugin_info (fixture->engine,
                                               extension_plugin);
}

static void
test_teardown (TestFixture    *fixture,
               gconstpointer  data)
{
  testing_engine_free (fixture->engine);
}

static void
test_runner  (TestFixture   *fixture,
              gconstpointer  data)
{
  g_assert_true (fixture->info != NULL);
  g_assert_true (peas_engine_load_plugin (fixture->engine, fixture->info));

  ((void (*) (PeasEngine *, PeasPluginInfo *)) data) (fixture->engine,
                                                      fixture->info);
}

static void
test_extension_garbage_collect (PeasEngine     *engine,
                                PeasPluginInfo *info)
{
  char **loaded_plugins;

  peas_engine_garbage_collect (engine);

  /* Check that we can collect the garbage when no plugins are loaded */
  g_assert_true (peas_engine_unload_plugin (engine, info));

  loaded_plugins = peas_engine_dup_loaded_plugins (engine);
  g_assert_cmpstr (loaded_plugins[0], ==, NULL);
  g_strfreev (loaded_plugins);

  peas_engine_garbage_collect (engine);
}

static void
test_extension_provides_valid (PeasEngine     *engine,
                               PeasPluginInfo *info)
{
  g_assert_true (peas_engine_provides_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE));
}

static void
test_extension_provides_invalid (PeasEngine     *engine,
                                 PeasPluginInfo *info)
{

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");

  /* Invalid GType */
  peas_engine_provides_extension (engine, info, G_TYPE_INVALID);


  /* GObject but not a GInterface */
  peas_engine_provides_extension (engine, info, PEAS_TYPE_ENGINE);


  /* Does not implement this GType */
  g_assert_true (!peas_engine_provides_extension (engine, info,
                                             INTROSPECTION_TYPE_UNIMPLEMENTED));

  /* Not loaded */
  g_assert_true (peas_engine_unload_plugin (engine, info));
  g_assert_true (!peas_engine_provides_extension (engine, info,
                                             INTROSPECTION_TYPE_CALLABLE));
}

static void
test_extension_create_valid (PeasEngine     *engine,
                             PeasPluginInfo *info)
{
  GObject *extension;

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  g_assert_true (G_IS_OBJECT (extension));
  g_assert_true (INTROSPECTION_IS_CALLABLE (extension));

  g_object_unref (extension);
}

static void
test_extension_create_valid_without_properties (PeasEngine     *engine,
                                                PeasPluginInfo *info)
{
  GObject *extension;

  extension =
    peas_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  0, NULL, NULL);

  g_assert_true (G_IS_OBJECT (extension));
  g_assert_true (INTROSPECTION_IS_CALLABLE (extension));

  g_object_unref (extension);
}

static void
test_extension_create_valid_with_properties (PeasEngine     *engine,
                                             PeasPluginInfo *info)
{
  GObject *extension;
  IntrospectionAbstract *abstract;
  GValue prop_values[1] = { G_VALUE_INIT };
  const char *prop_names[1] = { "abstract-property" };

  g_assert_true (peas_engine_load_plugin (engine, info));

  g_value_init (&prop_values[0], G_TYPE_INT);
  g_value_set_int (&prop_values[0], 47);
  extension =
      peas_engine_create_extension_with_properties (engine, info,
                                                    INTROSPECTION_TYPE_ABSTRACT,
                                                    G_N_ELEMENTS (prop_values),
                                                    prop_names,
                                                    prop_values);
  abstract = INTROSPECTION_ABSTRACT (extension);
  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, 47);

  g_object_unref (extension);
  g_value_unset (&prop_values[0]);
}

static void
test_extension_create_invalid (PeasEngine     *engine,
                               PeasPluginInfo *info)
{
  GObject *extension;

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*does not provide a 'IntrospectionUnimplemented' extension");
  testing_util_push_log_hook ("*type 'IntrospectionCallable' has no property named 'does-not-exist'");
  testing_util_push_log_hook ("*assertion*peas_plugin_info_is_loaded*failed");

  /* Invalid GType */
  extension = peas_engine_create_extension (engine, info, G_TYPE_INVALID, NULL);
  g_assert_true (!G_IS_OBJECT (extension));


  /* GObject but not a GInterface */
  extension = peas_engine_create_extension (engine, info, PEAS_TYPE_ENGINE, NULL);
  g_assert_true (!G_IS_OBJECT (extension));


  /* Does not implement this GType */
  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_UNIMPLEMENTED,
                                            NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Interface does not have a specified property */
  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            "does-not-exist", "",
                                            NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Not loaded */
  g_assert_true (peas_engine_unload_plugin (engine, info));
  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);
  g_assert_true (!G_IS_OBJECT (extension));
}

static void
test_extension_create_invalid_with_properties (PeasEngine     *engine,
                                               PeasPluginInfo *info)
{
  GObject *extension;
  GValue prop_values[1] = { G_VALUE_INIT };
  const char *prop_names[1] = { NULL };
  GValue prop_values2[1] = { G_VALUE_INIT };
  const char *prop_names2[1] = { "does-not-exist" };

  g_value_init (&prop_values[0], G_TYPE_STRING);
  g_value_set_string (&prop_values[0], "foo");

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*does not provide a 'IntrospectionUnimplemented' extension");
  testing_util_push_log_hook ("*property name*should not be NULL.");
  testing_util_push_log_hook ("*property value*should*initialized GValue.");
  testing_util_push_log_hook ("*assertion*peas_plugin_info_is_loaded*failed");

  /* Invalid GType */
  extension = peas_engine_create_extension_with_properties (engine, info,
                                                            G_TYPE_INVALID, 0,
                                                            NULL, NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  /* GObject but not a GInterface */
  extension = peas_engine_create_extension_with_properties (engine, info,
                                                            PEAS_TYPE_ENGINE, 0,
                                                            NULL, NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Does not implement this GType */
  extension =
    peas_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_UNIMPLEMENTED,
                                                  0, NULL, NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Interface has a NULL property name*/
  extension =
    peas_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  G_N_ELEMENTS (prop_names2),
                                                  prop_names, prop_values);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Interface has a not initialiazed GValue */
  extension =
    peas_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  G_N_ELEMENTS (prop_names2),
                                                  prop_names2, prop_values2);
  g_assert_true (!G_IS_OBJECT (extension));

  /* Not loaded */
  g_assert_true (peas_engine_unload_plugin (engine, info));
  extension =
    peas_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  0, NULL, NULL);
  g_assert_true (!G_IS_OBJECT (extension));

  g_value_unset (&prop_values[0]);

}

static void
test_extension_create_with_prerequisite (PeasEngine     *engine,
                                         PeasPluginInfo *info)
{
  GObject *extension;
  int prerequisite_property = -1;

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_HAS_PREREQUISITE,
                                            "prerequisite-property", 47,
                                            NULL);

  g_assert_true (INTROSPECTION_IS_HAS_PREREQUISITE (extension));
  g_assert_true (INTROSPECTION_IS_CALLABLE (extension));

  g_object_get (extension,
                "prerequisite-property", &prerequisite_property,
                NULL);
  g_assert_cmpint (prerequisite_property, ==, 47);

  g_object_unref (extension);
}

static void
test_extension_reload (PeasEngine     *engine,
                       PeasPluginInfo *info)
{
  int i;

  for (i = 0; i < 3; ++i)
    {
      g_assert_true (peas_engine_load_plugin (engine, info));
      g_assert_true (peas_engine_unload_plugin (engine, info));
    }
}

static void
test_extension_plugin_info (PeasEngine     *engine,
                            PeasPluginInfo *info)
{
  GObject *extension;
  IntrospectionBase *base;

  g_assert_true (peas_engine_load_plugin (engine, info));

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  base = INTROSPECTION_BASE (extension);

  g_assert_true (introspection_base_get_plugin_info (base) == info);

  g_object_unref (extension);
}

static void
test_extension_get_settings (PeasEngine     *engine,
                             PeasPluginInfo *info)
{
  GObject *extension;
  IntrospectionBase *base;
  GSettings *settings;

  g_assert_true (peas_engine_load_plugin (engine, info));

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  base = INTROSPECTION_BASE (extension);

  settings = introspection_base_get_settings (base);
  g_assert_nonnull (settings);
  g_assert_true (G_IS_SETTINGS (settings));

  g_object_unref (settings);
  g_object_unref (extension);
}

static void
test_extension_abstract (PeasEngine     *engine,
                         PeasPluginInfo *info)
{
  GObject *extension;
  IntrospectionAbstract *abstract;

  g_assert_true (peas_engine_load_plugin (engine, info));

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_ABSTRACT,
                                            "abstract-property", 47,
                                            NULL);

  abstract = INTROSPECTION_ABSTRACT (extension);

  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, 47);
  introspection_abstract_set_value (abstract, -22);
  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, -22);

  g_object_unref (extension);
}

static int
run_in_multiple_threads (GFunc    func,
                         gpointer user_data)
{
  int i, n_threads;
  GThreadPool *pool;
  GError *error = NULL;

  /* Avoid too many threads, but try to get some good contention */
  n_threads = g_get_num_processors () + 2;

  pool = g_thread_pool_new (func, user_data, n_threads, TRUE, &error);
  g_assert_no_error (error);

  for (i = 0; i < n_threads; ++i)
    {
      /* Cannot supply NULL as the data... */
      g_thread_pool_push (pool, GUINT_TO_POINTER (i + 1), &error);
      g_assert_no_error (error);
    }

  g_thread_pool_free (pool, FALSE, TRUE);

  return n_threads;
}

static void
multiple_threads_loaders_in_thread (guint    nth_thread,
                                    gboolean use_nonglobal_loaders)
{
  int i, j;
  PeasEngine *engine;
  PeasPluginInfo *info;
  GObject *extension;

  engine = testing_engine_new_full (use_nonglobal_loaders);
  peas_engine_enable_loader (engine, loader);

  info = peas_engine_get_plugin_info (engine, extension_plugin);
  g_assert_true (info != NULL);

  for (i = 0; i < 10; ++i)
    {
      g_assert_true (peas_engine_load_plugin (engine, info));

      for (j = 0; j < 50; ++j)
        {
          extension = peas_engine_create_extension (engine, info,
                                                    INTROSPECTION_TYPE_BASE,
                                                    NULL);

          g_assert_true (extension != NULL);
          g_object_unref (extension);
        }

      g_assert_true (peas_engine_unload_plugin (engine, info));
    }

  testing_engine_free (engine);
}

static void
test_extension_multiple_threads_global_loaders (PeasEngine     *engine,
                                                PeasPluginInfo *info)
{
  run_in_multiple_threads ((GFunc) multiple_threads_loaders_in_thread,
                           GINT_TO_POINTER (FALSE));
}

static void
test_extension_multiple_threads_nonglobal_loaders (PeasEngine     *engine,
                                                   PeasPluginInfo *info)
{
  run_in_multiple_threads ((GFunc) multiple_threads_loaders_in_thread,
                           GINT_TO_POINTER (TRUE));
}

static void
multiple_threads_callbacks_in_thread (guint                     nth_thread,
                                      IntrospectionActivatable *activatable)
{
  int i;

  for (i = 0; i < 100; ++i)
    introspection_activatable_update_state (activatable);
}

static void
test_extension_multiple_threads_callbacks (PeasEngine     *engine,
                                           PeasPluginInfo *info)
{
  GObject *extension;
  int n_threads, update_count;

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_ACTIVATABLE,
                                            NULL);

  n_threads = run_in_multiple_threads ((GFunc) multiple_threads_callbacks_in_thread,
                                       extension);

  g_object_get (extension, "update-count", &update_count, NULL);
  g_assert_cmpint (update_count, ==, n_threads * 100);

  g_object_unref (extension);
}

#define _EXTENSION_TEST(loader, path, ftest) \
  G_STMT_START { \
    char *full_path = g_strdup_printf (EXTENSION_TEST_NAME (%s, "%s"), \
                                       loader, path);                  \
                                                                       \
    g_test_add (full_path, TestFixture,                                \
                (gpointer) test_extension_##ftest,                     \
                test_setup, test_runner, test_teardown);               \
                                                                       \
    g_free (full_path);                                                \
  } G_STMT_END

void
testing_extension_basic (const char *loader_)
{
  int i, j;
  char *loader_name;
  PeasEngine *engine;

  loader = g_strdup (loader_);

  loader_name = g_new0 (char, strlen (loader) + 1);
  for (i = 0, j = 0; loader[i] != '\0'; ++i)
    {
      if (loader[i] != '.')
        loader_name[j++] = loader[i];
    }

  extension_plugin = g_strdup_printf ("extension-%s", loader_name);
  g_free (loader_name);

  engine = testing_engine_new ();
  peas_engine_enable_loader (engine, loader);

  /* Check that the loaders are created lazily */
  g_assert_true (g_type_from_name ("PeasPluginLoader") == G_TYPE_INVALID);

  testing_engine_free (engine);


  _EXTENSION_TEST (loader, "garbage-collect", garbage_collect);

  _EXTENSION_TEST (loader, "provides-valid", provides_valid);
  _EXTENSION_TEST (loader, "provides-invalid", provides_invalid);

  _EXTENSION_TEST (loader, "create-valid", create_valid);
  _EXTENSION_TEST (loader, "create-valid-without-properties",
                   create_valid_without_properties);
  _EXTENSION_TEST (loader, "create-valid-with-properties",
                   create_valid_with_properties);
  _EXTENSION_TEST (loader, "create-invalid", create_invalid);
  _EXTENSION_TEST (loader, "create-invalid-with-properties",
                   create_invalid_with_properties);
  _EXTENSION_TEST (loader, "create-with-prerequisite", create_with_prerequisite);

  _EXTENSION_TEST (loader, "reload", reload);

  _EXTENSION_TEST (loader, "plugin-info", plugin_info);
  _EXTENSION_TEST (loader, "get-settings", get_settings);

  _EXTENSION_TEST (loader, "abstract", abstract);

  if (g_strcmp0 (loader, "gjs") != 0)
    {
      _EXTENSION_TEST (loader, "multiple-threads/global-loaders",
                       multiple_threads_global_loaders);
      _EXTENSION_TEST (loader, "multiple-threads/nonglobal-loaders",
                       multiple_threads_nonglobal_loaders);
    }

  /* Not needed for C plugins as they are independent of libpeas */
  if (g_strcmp0 (loader, "c") != 0 && g_strcmp0 (loader, "gjs") != 0)
    {
      _EXTENSION_TEST (loader, "multiple-threads/callbacks",
                       multiple_threads_callbacks);
    }
}

void
testing_extension_add (const char    *path,
                       GTestDataFunc  func)
{
  g_test_add (path, TestFixture, (gpointer) func,
              test_setup, test_runner, test_teardown);
}

int
testing_extension_run_tests (void)
{
  int retval;

  retval = testing_run_tests ();

  g_free (extension_plugin);
  g_free (loader);

  return retval;
}
