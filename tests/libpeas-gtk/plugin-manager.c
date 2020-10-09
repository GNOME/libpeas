/*
 * plugin-manager.c
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
 */

#include "config.h"

#include <gtk/gtk.h>

#include <libpeas/peas.h>
#include <libpeas/peas-i18n-priv.h>
#include <libpeas-gtk/peas-gtk.h>

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "testing/testing.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  PeasEngine *engine;
  GtkWidget *window;
  PeasGtkPluginManager *manager;
  PeasGtkPluginManagerView *view;
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkWidget *about_button;
  GtkWidget *configure_button;
};

static void
notify_model_cb (GtkTreeView *view,
                 GParamSpec  *pspec,
                 TestFixture *fixture)
{
  fixture->model = gtk_tree_view_get_model (GTK_TREE_VIEW (fixture->view));
}

static void
test_setup (TestFixture   *fixture,
            gconstpointer  data)
{
  fixture->engine = testing_engine_new ();
  fixture->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  fixture->manager = PEAS_GTK_PLUGIN_MANAGER (peas_gtk_plugin_manager_new (NULL));
  fixture->view = PEAS_GTK_PLUGIN_MANAGER_VIEW (peas_gtk_plugin_manager_get_view (fixture->manager));
  fixture->selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (fixture->view));

  gtk_container_add (GTK_CONTAINER (fixture->window),
                     GTK_WIDGET (fixture->manager));

  g_signal_connect (fixture->view,
                    "notify::model",
                    G_CALLBACK (notify_model_cb),
                    fixture);

  /* Set the model */
  g_object_notify (G_OBJECT (fixture->view), "model");

  fixture->about_button = gtk_test_find_widget (fixture->window,
                                                _("About"),
                                                GTK_TYPE_BUTTON);
  g_assert (GTK_IS_BUTTON (fixture->about_button));

  fixture->configure_button = gtk_test_find_widget (fixture->window,
                                                    _("Preferences"),
                                                    GTK_TYPE_BUTTON);
  g_assert (GTK_IS_BUTTON (fixture->configure_button));
}

static void
test_teardown (TestFixture   *fixture,
               gconstpointer  data)
{
  gtk_widget_destroy (GTK_WIDGET (fixture->window));

  testing_engine_free (fixture->engine);
}

static void
test_runner (TestFixture   *fixture,
             gconstpointer  data)
{
  ((void (*) (TestFixture *)) data) (fixture);
}

static GtkWidget *
find_window_by_title (GtkWindow   *window,
                      const gchar *title)
{
  GtkWindowGroup *group;
  GList *windows;
  GList *l;
  GtkWidget *found_window = NULL;

  group = gtk_window_get_group (window);
  windows = gtk_window_group_list_windows (group);

  for (l = windows; l != NULL; l = l->next)
    {
      if (g_strcmp0 (gtk_window_get_title (l->data), title) == 0)
        {
          found_window = l->data;
          break;
        }
    }

  g_list_free (windows);

  g_assert (GTK_IS_WINDOW (found_window));

  return found_window;
}

static void
test_gtk_plugin_manager_about_button_sensitivity (TestFixture *fixture)
{
  GtkTreeIter iter;
  PeasPluginInfo *info;

  testing_util_push_log_hook ("Could not find plugin 'does-not-exist'*");

  /* Must come first otherwise the first iter may
   * be after a revealed builtin plugin
   */
  peas_gtk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  /* Causes the plugin to become unavailable */
  info = peas_engine_get_plugin_info (fixture->engine, "unavailable");
  peas_engine_load_plugin (fixture->engine, info);

  g_assert (gtk_tree_model_get_iter_first (fixture->model, &iter));

  do
    {
      gtk_tree_selection_select_iter (fixture->selection, &iter);

      g_assert (gtk_widget_get_sensitive (fixture->about_button));
    }
  while (gtk_tree_model_iter_next (fixture->model, &iter));
}

static void
test_gtk_plugin_manager_configure_button_sensitivity (TestFixture *fixture)
{
  GtkTreeIter iter;
  PeasPluginInfo *info;

  testing_util_push_log_hook ("Could not find plugin 'does-not-exist'*");

  /* Must come first otherwise the first iter may
   * be after a revealed builtin plugin
   */
  peas_gtk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  /* So we can configure them */
  info = peas_engine_get_plugin_info (fixture->engine, "builtin-configurable");
  peas_engine_load_plugin (fixture->engine, info);
  info = peas_engine_get_plugin_info (fixture->engine, "configurable");
  peas_engine_load_plugin (fixture->engine, info);

  /* Causes the plugin to become unavailable */
  info = peas_engine_get_plugin_info (fixture->engine, "unavailable");
  peas_engine_load_plugin (fixture->engine, info);

  g_assert (gtk_tree_model_get_iter_first (fixture->model, &iter));

  do
    {
      gboolean sensitive;

      gtk_tree_selection_select_iter (fixture->selection, &iter);

      info = testing_get_plugin_info_for_iter (fixture->view, &iter);

      if (!peas_plugin_info_is_loaded (info))
        {
          sensitive = FALSE;
        }
      else
        {
          sensitive = peas_engine_provides_extension (fixture->engine, info,
                                                      PEAS_GTK_TYPE_CONFIGURABLE);
        }

      g_assert_cmpint (gtk_widget_get_sensitive (fixture->configure_button),
                       ==, sensitive);
    }
  while (gtk_tree_model_iter_next (fixture->model, &iter));
}

static void
test_gtk_plugin_manager_plugin_loaded (TestFixture *fixture)
{
  GtkTreeIter iter;
  PeasPluginInfo *info;

  info = peas_engine_get_plugin_info (fixture->engine, "configurable");
  testing_get_iter_for_plugin_info (fixture->view, info, &iter);

  gtk_tree_selection_select_iter (fixture->selection, &iter);

  g_assert (!gtk_widget_is_sensitive (fixture->configure_button));
  peas_engine_load_plugin (fixture->engine, info);
  g_assert (gtk_widget_is_sensitive (fixture->configure_button));
}

static void
test_gtk_plugin_manager_plugin_unloaded (TestFixture *fixture)
{
  GtkTreeIter iter;
  PeasPluginInfo *info;

  test_gtk_plugin_manager_plugin_loaded (fixture);

  info = peas_engine_get_plugin_info (fixture->engine, "configurable");
  testing_get_iter_for_plugin_info (fixture->view, info, &iter);

  g_assert (gtk_widget_is_sensitive (fixture->configure_button));
  peas_engine_unload_plugin (fixture->engine, info);
  g_assert (!gtk_widget_is_sensitive (fixture->configure_button));
}

static void
test_gtk_plugin_manager_about_dialog (TestFixture *fixture)
{
  gint i;
  GtkTreeIter iter;
  GtkWidget *window;
  PeasPluginInfo *info;
  const gchar **authors_plugin;
  const gchar * const *authors_dialog;

  /* Full Info is a builtin plugin */
  peas_gtk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  info = peas_engine_get_plugin_info (fixture->engine, "full-info");

  testing_get_iter_for_plugin_info (fixture->view, info, &iter);
  gtk_tree_selection_select_iter (fixture->selection, &iter);

  /* Must be first so we the window is added to the window group */
  gtk_button_clicked (GTK_BUTTON (fixture->about_button));

  window = find_window_by_title (GTK_WINDOW (fixture->window),
                                 "About Full Info");
  g_assert (GTK_IS_ABOUT_DIALOG (window));


  g_assert_cmpstr (gtk_about_dialog_get_program_name (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_name (info));
  g_assert_cmpstr (gtk_about_dialog_get_copyright (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_copyright (info));
  g_assert_cmpstr (gtk_about_dialog_get_website (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_website (info));
  g_assert_cmpstr (gtk_about_dialog_get_logo_icon_name (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_icon_name (info));
  g_assert_cmpstr (gtk_about_dialog_get_version (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_version (info));
  g_assert_cmpstr (gtk_about_dialog_get_comments (GTK_ABOUT_DIALOG (window)),
                   ==, peas_plugin_info_get_description (info));

  authors_plugin = peas_plugin_info_get_authors (info);
  authors_dialog = gtk_about_dialog_get_authors (GTK_ABOUT_DIALOG (window));

  for (i = 0; authors_plugin[i] == NULL && authors_dialog[i] == NULL; ++i)
    g_assert_cmpstr (authors_plugin[i], ==, authors_dialog[i]);

  gtk_widget_destroy (window);
}

static void
test_gtk_plugin_manager_configure_dialog (TestFixture *fixture)
{
  GtkTreeIter iter;
  GtkWidget *window;
  PeasPluginInfo *info;
  GList *list;
  GList *list_it;
  GtkWidget *content;
  GtkWidget *label = NULL;
  GtkWidget *close_button = NULL;
  GtkWidget *help_button = NULL;

  info = peas_engine_get_plugin_info (fixture->engine, "configurable");

  peas_engine_load_plugin (fixture->engine, info);

  testing_get_iter_for_plugin_info (fixture->view, info, &iter);
  gtk_tree_selection_select_iter (fixture->selection, &iter);

  /* Must be first so the window is added to the window group */
  gtk_button_clicked (GTK_BUTTON (fixture->configure_button));

  window = find_window_by_title (GTK_WINDOW (fixture->window), "Configurable");
  g_assert (GTK_IS_DIALOG (window));

  content = gtk_dialog_get_content_area (GTK_DIALOG (window));
  list = gtk_container_get_children (GTK_CONTAINER (content));

  for (list_it = list; list_it != NULL; list_it = list_it->next)
    {
      if (GTK_IS_LABEL (list_it->data))
        {
          const gchar *text = gtk_label_get_text (GTK_LABEL (list_it->data));

          if (g_strcmp0 (text, "Hello, World!") == 0)
            label = GTK_WIDGET (list_it->data);
        }
    }

  g_assert (label != NULL);

  g_list_free (list);


  close_button = gtk_dialog_get_widget_for_response (GTK_DIALOG (window),
                                                     GTK_RESPONSE_CLOSE);
  g_assert (close_button != NULL);

  help_button = gtk_dialog_get_widget_for_response (GTK_DIALOG (window),
                                                    GTK_RESPONSE_HELP);
  g_assert (help_button != NULL);

  gtk_widget_destroy (window);
}

static void
test_gtk_plugin_manager_gtkbuilder (void)
{
  GtkBuilder *builder;
  GError *error = NULL;
  PeasGtkPluginManager *manager;
  PeasGtkPluginManagerView *view;
  static const gchar *gtkbuilder_string =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<interface>\n"
    "<object class='PeasGtkPluginManagerView' id='view'>\n"
    "  <property name='show-builtin'>True</property>\n"
    "</object>\n"
    "<object class='PeasGtkPluginManager' id='manager'>\n"
    "  <property name='view'>view</property>\n"
    "</object>\n"
    "</interface>";

  builder = gtk_builder_new ();

  gtk_builder_add_from_string (builder, gtkbuilder_string, -1, &error);
  g_assert_no_error (error);

  manager = PEAS_GTK_PLUGIN_MANAGER (gtk_builder_get_object (builder, "manager"));
  g_assert (PEAS_GTK_IS_PLUGIN_MANAGER (manager));

  view = PEAS_GTK_PLUGIN_MANAGER_VIEW (peas_gtk_plugin_manager_get_view (manager));

  g_assert (G_OBJECT (view) == gtk_builder_get_object (builder, "view"));

  g_assert (peas_gtk_plugin_manager_view_get_show_builtin (view));

  /* Freeing the builder will free the objects */
  g_object_unref (builder);
}

int
main (int    argc,
      char **argv)
{
  testing_init (&argc, &argv);

#ifdef G_OS_WIN32
  SetThreadLocale (MAKELCID (MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
#endif

#define TEST(path, ftest) \
  g_test_add ("/gtk/plugin-manager/" path, TestFixture, \
              (gpointer) test_gtk_plugin_manager_##ftest, \
              test_setup, test_runner, test_teardown)

#define TEST_FUNC(path, ftest) \
  g_test_add_func ("/gtk/plugin-manager/" path, \
                   test_gtk_plugin_manager_##ftest)

  TEST ("about-button-sensitivity", about_button_sensitivity);
  TEST ("configure-button-sensitivity", configure_button_sensitivity);

  TEST ("plugin-loaded", plugin_loaded);
  TEST ("plugin-unloaded", plugin_unloaded);

  TEST ("about-dialog", about_dialog);
  TEST ("configure-dialog", configure_dialog);

  TEST_FUNC ("gtkbuilder", gtkbuilder);

#undef TEST

  return testing_run_tests ();
}
