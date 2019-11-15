/*
 * testing.h
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

#ifndef __TESTING_H__
#define __TESTING_H__

#include <libpeas/peas.h>
#include <libpeas-gtk/peas-gtk.h>
#include <testing-util.h>

G_BEGIN_DECLS

PEAS_TEST_EXPORT
void            testing_init                     (gint                       *argc,
                                                  gchar                    ***argv);

PEAS_TEST_EXPORT
PeasEngine     *testing_engine_new               (void);

PEAS_TEST_EXPORT
PeasPluginInfo *testing_get_plugin_info_for_iter (PeasGtkPluginManagerView   *view,
                                                  GtkTreeIter                *iter);
PEAS_TEST_EXPORT
gboolean        testing_get_iter_for_plugin_info (PeasGtkPluginManagerView   *view,
                                                  PeasPluginInfo             *info,
                                                  GtkTreeIter                *iter);

PEAS_TEST_EXPORT
void            testing_show_widget              (gpointer                    widget);

/* libtesting-util functions which do not need to be overridden */
#define testing_engine_free testing_util_engine_free
#define testing_run_tests   testing_util_run_tests

G_END_DECLS

#endif /* __TESTING_H__ */
