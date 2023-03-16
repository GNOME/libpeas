/*
 * peas-test-common.h
 * This file is part of libpeas
 *
 * Copyright (C) 2019 - Chun-wei Fan
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

#ifdef _MSC_VER
# define PEAS_TEST_EXPORT __declspec (dllexport)
#else
# define PEAS_TEST_EXPORT
#endif