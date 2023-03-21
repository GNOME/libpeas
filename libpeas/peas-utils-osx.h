/*
 * peas-utils-osx.h
 * This file is part of libpeas
 *
 * Copyright (C) 2008 Ignacio Casal Quinteiro
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
 * Author: Tom Schoonjans <Tom.Schoonjans@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#pragma once

#include <glib.h>

G_BEGIN_DECLS

char *peas_dirs_os_x_get_bundle_resource_dir (void);
char *peas_dirs_os_x_get_resource_dir        (const char *subdir,
                                              const char *default_dir);
char *peas_dirs_os_x_get_data_dir            (void);
char *peas_dirs_os_x_get_lib_dir             (void);
char *peas_dirs_os_x_get_locale_dir          (void);
void  peas_open_url_osx                      (const char *uri);

G_END_DECLS
