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
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <libpeas/peas-engine.h>
#include <testing-util.h>

G_BEGIN_DECLS

PEAS_TEST_EXPORT
void        testing_init             (int    *argc,
                                      char ***argv);

PEAS_TEST_EXPORT
PeasEngine *testing_engine_new_full  (gboolean nonglobal_loaders);

#define testing_engine_new() (testing_engine_new_full (FALSE))

/* libtesting-util functions which do not need to be overridden */
#define testing_engine_free testing_util_engine_free
#define testing_run_tests   testing_util_run_tests

G_END_DECLS
