/*
 * peas-macros.h
 * This file is part of libpeas, based on GLib's gmacros.h and gversionmacros.h.
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

#ifndef __PEAS_MACROS_H__
#define __PEAS_MACROS_H__

#include <glib.h>

#ifndef _PEAS_EXTERN
#define _PEAS_EXTERN
#endif

#ifdef PEAS_DISABLE_DEPRECATION_WARNINGS
#define PEAS_DEPRECATED _PEAS_EXTERN
#define PEAS_DEPRECATED_FOR(f) _PEAS_EXTERN
#define PEAS_UNAVAILABLE(maj,min) _PEAS_EXTERN
#else
#define PEAS_DEPRECATED G_DEPRECATED _PEAS_EXTERN
#define PEAS_DEPRECATED_FOR(f) G_DEPRECATED(f) _PEAS_EXTERN
#define PEAS_UNAVAILABLE(maj,min) G_UNAVAILABLE(maj,min) _PEAS_EXTERN
#endif

#define PEAS_API _PEAS_EXTERN

#endif /* __PEAS_MACROS_H__ */