/*
 * peas-utils-osx.m
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
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "peas-utils-osx.h"

#import <Foundation/Foundation.h>

#import <AppKit/AppKit.h>

void
peas_open_url_osx (const gchar *uri)
{
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:uri]]];
}

gchar *
peas_dirs_os_x_get_bundle_resource_dir (void)
{
  NSAutoreleasePool *pool;
  gchar *str = NULL;
  NSString *path;

  pool = [[NSAutoreleasePool alloc] init];

  if ([[NSBundle mainBundle] bundleIdentifier] == nil)
    {
      [pool release];
      return NULL;
    }

  path = [[NSBundle mainBundle] resourcePath];

  if (!path)
    {
      [pool release];
      return NULL;
    }

  str = g_strdup ([path UTF8String]);
  [pool release];
  return str;
}

gchar *
peas_dirs_os_x_get_resource_dir (const gchar *subdir,
                                 const gchar *default_dir)
{
  gchar *res_dir;
  gchar *ret;

  res_dir = peas_dirs_os_x_get_bundle_resource_dir ();

  if (res_dir == NULL)
    {
      ret = g_build_filename (default_dir, "libpeas-1.0", NULL);
    }
  else
    {
      ret = g_build_filename (res_dir, subdir, "libpeas-1.0", NULL);
      g_free (res_dir);
    }

  return ret;
}

gchar *
peas_dirs_os_x_get_data_dir (void)
{
  return peas_dirs_os_x_get_resource_dir ("share", DATADIR);
}

gchar *
peas_dirs_os_x_get_lib_dir (void)
{
  return peas_dirs_os_x_get_resource_dir ("lib", LIBDIR);
}

gchar *
peas_dirs_os_x_get_locale_dir (void)
{
  gchar *res_dir;
  gchar *ret;

  res_dir = peas_dirs_os_x_get_bundle_resource_dir ();

  if (res_dir == NULL)
    {
      ret = g_build_filename (DATADIR, "locale", NULL);
    }
  else
    {
      ret = g_build_filename (res_dir, "share", "locale", NULL);
      g_free (res_dir);
    }

  return ret;
}
