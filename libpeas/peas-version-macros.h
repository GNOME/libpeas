/*
 * peas-version-macros.h
 * This file is part of libpeas
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

#ifndef __PEAS_VERSION_MACROS_H__
#define __PEAS_VERSION_MACROS_H__

#include <glib.h>

#include "peas-version.h"

#ifndef _PEAS_EXTERN
# define _PEAS_EXTERN extern
#endif

#ifdef PEAS_DISABLE_DEPRECATION_WARNINGS
# define PEAS_DEPRECATED _PEAS_EXTERN
# define PEAS_DEPRECATED_FOR(f) _PEAS_EXTERN
# define PEAS_UNAVAILABLE(maj,min) _PEAS_EXTERN
#else
# define PEAS_DEPRECATED G_DEPRECATED _PEAS_EXTERN
# define PEAS_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f) _PEAS_EXTERN
# define PEAS_UNAVAILABLE(maj,min) G_UNAVAILABLE(maj,min) _PEAS_EXTERN
#endif

#define PEAS_VERSION_1_22 (G_ENCODE_VERSION (1, 22))
#define PEAS_VERSION_1_24 (G_ENCODE_VERSION (1, 24))

#if (PEAS_MINOR_VERSION == 99)
# define PEAS_VERSION_CUR_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION + 1, 0))
#elif (PEAS_MINOR_VERSION % 2)
# define PEAS_VERSION_CUR_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION, PEAS_MINOR_VERSION + 1))
#else
# define PEAS_VERSION_CUR_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION, PEAS_MINOR_VERSION))
#endif

#if (PEAS_MINOR_VERSION == 99)
# define PEAS_VERSION_PREV_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION + 1, 0))
#elif (PEAS_MINOR_VERSION % 2)
# define PEAS_VERSION_PREV_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION, PEAS_MINOR_VERSION - 1))
#else
# define PEAS_VERSION_PREV_STABLE (G_ENCODE_VERSION (PEAS_MAJOR_VERSION, PEAS_MINOR_VERSION - 2))
#endif

/**
 * PEAS_VERSION_MIN_REQUIRED:
 *
 * A macro that should be defined by the user prior to including
 * the peas.h header.
 *
 * The definition should be one of the predefined PEAS version
 * macros: %PEAS_VERSION_1_22, ...
 *
 * This macro defines the lower bound for the Peas API to use.
 *
 * If a function has been deprecated in a newer version of Peas,
 * it is possible to use this symbol to avoid the compiler warnings
 * without disabling warning for every deprecated function.
 *
 * Since: 1.24
 */
#ifndef PEAS_VERSION_MIN_REQUIRED
# define PEAS_VERSION_MIN_REQUIRED (PEAS_VERSION_CUR_STABLE)
#endif

/**
 * PEAS_VERSION_MAX_ALLOWED:
 *
 * A macro that should be defined by the user prior to including
 * the peas.h header.

 * The definition should be one of the predefined Peas version
 * macros: %PEAS_VERSION_1_0, %PEAS_VERSION_1_2,...
 *
 * This macro defines the upper bound for the PEAS API to use.
 *
 * If a function has been introduced in a newer version of Peas,
 * it is possible to use this symbol to get compiler warnings when
 * trying to use that function.
 *
 * Since: 1.24
 */
#ifndef PEAS_VERSION_MAX_ALLOWED
# if PEAS_VERSION_MIN_REQUIRED > PEAS_VERSION_PREV_STABLE
#  define PEAS_VERSION_MAX_ALLOWED (PEAS_VERSION_MIN_REQUIRED)
# else
#  define PEAS_VERSION_MAX_ALLOWED (PEAS_VERSION_CUR_STABLE)
# endif
#endif

#if PEAS_VERSION_MAX_ALLOWED < PEAS_VERSION_MIN_REQUIRED
#error "PEAS_VERSION_MAX_ALLOWED must be >= PEAS_VERSION_MIN_REQUIRED"
#endif
#if PEAS_VERSION_MIN_REQUIRED < PEAS_VERSION_1_22
#error "PEAS_VERSION_MIN_REQUIRED must be >= PEAS_VERSION_1_22"
#endif

#define PEAS_AVAILABLE_IN_ALL                   _PEAS_EXTERN

#if PEAS_VERSION_MIN_REQUIRED >= PEAS_VERSION_1_22
# define PEAS_DEPRECATED_IN_1_22                PEAS_DEPRECATED
# define PEAS_DEPRECATED_IN_1_22_FOR(f)         PEAS_DEPRECATED_FOR(f)
#else
# define PEAS_DEPRECATED_IN_1_22                _PEAS_EXTERN
# define PEAS_DEPRECATED_IN_1_22_FOR(f)         _PEAS_EXTERN
#endif

#if PEAS_VERSION_MAX_ALLOWED < PEAS_VERSION_1_22
# define PEAS_AVAILABLE_IN_1_22                 PEAS_UNAVAILABLE(1, 22)
#else
# define PEAS_AVAILABLE_IN_1_22                 _PEAS_EXTERN
#endif

#if PEAS_VERSION_MIN_REQUIRED >= PEAS_VERSION_1_24
# define PEAS_DEPRECATED_IN_1_24                PEAS_DEPRECATED
# define PEAS_DEPRECATED_IN_1_24_FOR(f)         PEAS_DEPRECATED_FOR(f)
#else
# define PEAS_DEPRECATED_IN_1_24                _PEAS_EXTERN
# define PEAS_DEPRECATED_IN_1_24_FOR(f)         _PEAS_EXTERN
#endif

#if PEAS_VERSION_MAX_ALLOWED < PEAS_VERSION_1_24
# define PEAS_AVAILABLE_IN_1_24                 PEAS_UNAVAILABLE(1, 24)
#else
# define PEAS_AVAILABLE_IN_1_24                 _PEAS_EXTERN
#endif

#endif /* __PEAS_VERSION_MACROS_H__ */
