/*
 * extension-py.c
 * This file is part of libpeas
 *
 * Copyright (C) 2011 - Steve Frécinaux
 * Copyright (C) 2011-2013 - Garrett Regier
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

#include <pygobject.h>

#include "libpeas/peas-engine-priv.h"

#include "testing/testing-extension.h"

#include "introspection/introspection-activatable.h"
#include "introspection/introspection-base.h"

#define PY_LOADER      python
#define PY_LOADER_STR  G_STRINGIFY (PY_LOADER)


static void
test_extension_py_instance_refcount (PeasEngine     *engine,
                                     PeasPluginInfo *info)
{
  GObject *extension;

  extension = peas_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  g_assert_true (G_IS_OBJECT (extension));

  g_object_add_weak_pointer (extension, (gpointer *) &extension);

  g_assert_cmpint (extension->ref_count, ==, 2);

  g_object_unref (extension);
  g_assert_true (extension == NULL);
}

static void
test_extension_py_activatable_subject_refcount (PeasEngine     *engine,
                                                PeasPluginInfo *info)
{
  GObject *extension;
  GObject *object;
  PyObject *wrapper;

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

  /* The python wrapper created around our dummy object should have increased
   * its refcount by 1.
   */
  g_assert_cmpint (object->ref_count, ==, 2);

  /* Ensure the python wrapper is only reffed once, by the extension */
  wrapper = g_object_get_data (object, "PyGObject::wrapper");
  g_assert_cmpint (wrapper->ob_refcnt, ==, 1);

  g_assert_cmpint (G_OBJECT (extension)->ref_count, ==, 2);
  g_object_unref (extension);

  /* We unreffed the extension, so it should have been destroyed and our dummy
   * object refcount should be back to 1. */
  g_assert_cmpint (object->ref_count, ==, 1);

  g_object_unref (object);
}

static void
test_extension_py_nonexistent (PeasEngine *engine)
{
  PeasPluginInfo *info;

  testing_util_push_log_hook ("Error importing plugin 'extension-"
                              PY_LOADER_STR "-nonexistent'*");
  testing_util_push_log_hook ("Error loading plugin 'extension-"
                              PY_LOADER_STR "-nonexistent'");

  info = peas_engine_get_plugin_info (engine,
                                      "extension-" PY_LOADER_STR "-nonexistent");

  g_assert_true (!peas_engine_load_plugin (engine, info));
}

static void
test_extension_py_already_initialized (void)
{
  g_test_trap_subprocess (EXTENSION_TEST_NAME (PY_LOADER,
                                               "already-initialized/subprocess"),
                          0, G_TEST_SUBPROCESS_INHERIT_STDERR);
  g_test_trap_assert_passed ();
  g_test_trap_assert_stderr ("");
}

static void
test_extension_py_already_initialized_subprocess (void)
{
  PeasEngine *engine;
  PyObject *module, *dict, *pyengine, *result;

  /* Check that python has not been initialized yet */
  g_assert_true (!Py_IsInitialized ());
  Py_InitializeEx (FALSE);

  /* Initialize PyGObject */
  pygobject_init (PYGOBJECT_MAJOR_VERSION,
                  PYGOBJECT_MINOR_VERSION,
                  PYGOBJECT_MICRO_VERSION);
  g_assert_true (!PyErr_Occurred ());

  engine = testing_engine_new ();
  peas_engine_enable_loader (engine, PY_LOADER_STR);

  module = PyImport_AddModule ("__main__");
  dict = PyModule_GetDict (module);

  pyengine = pygobject_new (G_OBJECT (engine));
  g_assert_true (PyDict_SetItemString (dict, "engine", pyengine) == 0);
  Py_DECREF (pyengine);

  result = PyRun_String ("plugin_name = 'extension-" PY_LOADER_STR "'\n"
                         "info = engine.get_plugin_info(plugin_name)\n"
                         "assert engine.load_plugin(info)\n",
                         Py_file_input, dict, dict);
  Py_XDECREF (result);

  g_assert_true (!PyErr_Occurred ());

  PyDict_Clear (dict);
  testing_engine_free (engine);

  _peas_engine_shutdown ();

  /* Should still be initialized */
  g_assert_true (Py_IsInitialized ());
  Py_Finalize ();
}

int
main (int   argc,
      char *argv[])
{
  testing_init (&argc, &argv);

  /* Only test the basics */
  testing_extension_basic (PY_LOADER_STR);

#undef EXTENSION_TEST
#undef EXTENSION_TEST_FUNC

#define EXTENSION_TEST(loader, path, func) \
  testing_extension_add (EXTENSION_TEST_NAME (loader, path), \
                         (gpointer) test_extension_py_##func)

#define EXTENSION_TEST_FUNC(loader, path, func) \
  g_test_add_func (EXTENSION_TEST_NAME (loader, path), \
                   (gpointer) test_extension_py_##func)

  EXTENSION_TEST (PY_LOADER, "instance-refcount", instance_refcount);
  EXTENSION_TEST (PY_LOADER, "activatable-subject-refcount",
                  activatable_subject_refcount);

  EXTENSION_TEST (PY_LOADER, "nonexistent", nonexistent);

  EXTENSION_TEST_FUNC (PY_LOADER, "already-initialized", already_initialized);
  EXTENSION_TEST_FUNC (PY_LOADER, "already-initialized/subprocess",
                       already_initialized_subprocess);

  return testing_extension_run_tests ();
}
