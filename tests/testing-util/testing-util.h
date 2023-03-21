/*
 * testing-util.h
 * This file is part of libpeas
 *
 * Copyright (C) 2011 - Garrett Regier
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

#include "peas-test-common.h"

G_BEGIN_DECLS

PEAS_TEST_EXPORT
void        testing_util_envars          (void);

PEAS_TEST_EXPORT
void        testing_util_init            (void);


PEAS_TEST_EXPORT
PeasEngine *testing_util_engine_new_full (gboolean    nonglobal_loaders);

PEAS_TEST_EXPORT
void        testing_util_engine_free     (PeasEngine *engine);


PEAS_TEST_EXPORT
int         testing_util_run_tests       (void);


PEAS_TEST_EXPORT
void        testing_util_push_log_hook   (const char *pattern);

PEAS_TEST_EXPORT
void        testing_util_pop_log_hook    (void);

PEAS_TEST_EXPORT
void        testing_util_pop_log_hooks   (void);


#define testing_util_engine_new() (testing_util_engine_new_full (FALSE))

G_END_DECLS
