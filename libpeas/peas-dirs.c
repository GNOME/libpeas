/*
 * peas-dirs.c
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
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "config.h"

#include "peas-dirs.h"

#ifdef OS_OSX
#include "peas-utils-osx.h"
#endif

#ifdef G_OS_WIN32
// inspired by gobject-introspection...

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static HMODULE libpeas_dll = NULL;

#ifdef DLL_EXPORT

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    libpeas_dll = hinstDLL;
  return TRUE;
}

#endif
#endif

char *
peas_dirs_get_data_dir (void)
{
  char *data_dir;

#ifdef G_OS_WIN32
  char *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libpeas_dll);

  data_dir = g_build_filename (win32_dir, "share", "libpeas-2", NULL);
  g_free (win32_dir);
#elif defined (OS_OSX)
  data_dir = peas_dirs_os_x_get_data_dir ();
#else
  data_dir = g_build_filename (DATADIR, "libpeas-2", NULL);
#endif

  return data_dir;
}

char *
peas_dirs_get_lib_dir (void)
{
  char *lib_dir;

#ifdef G_OS_WIN32
  char *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libpeas_dll);

  lib_dir = g_build_filename (win32_dir, "lib", "libpeas-2", NULL);
  g_free (win32_dir);
#elif defined (OS_OSX)
  lib_dir = peas_dirs_os_x_get_lib_dir ();
#else
  lib_dir = g_build_filename (LIBDIR, "libpeas-2", NULL);
#endif

  return lib_dir;
}

char *
peas_dirs_get_plugin_loader_dir (const char *loader_name)
{
  const char *env_var;
  char *lib_dir;
  char *loader_dir;

  env_var = g_getenv ("PEAS_PLUGIN_LOADERS_DIR");
  if (env_var != NULL)
    return g_build_filename (env_var, loader_name, NULL);

  lib_dir = peas_dirs_get_lib_dir ();
  loader_dir = g_build_filename (lib_dir, "loaders", NULL);

  g_free (lib_dir);

  return loader_dir;
}

char *
peas_dirs_get_locale_dir (void)
{
  char *locale_dir;

#ifdef G_OS_WIN32
  char *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libpeas_dll);

  locale_dir = g_build_filename (win32_dir, "share", "locale", NULL);

  g_free (win32_dir);
#elif defined (OS_OSX)
  locale_dir = peas_dirs_os_x_get_locale_dir ();
#else
  locale_dir = g_build_filename (DATADIR, "locale", NULL);
#endif

  return locale_dir;
}
