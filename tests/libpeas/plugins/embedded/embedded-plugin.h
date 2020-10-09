/*
 * embedded-plugin.h
 * This file is part of libpeas
 *
 * Copyright (C) 2015 - Garrett Regier
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

#ifndef __TESTING_EMBEDDED_PLUGIN_H__
#define __TESTING_EMBEDDED_PLUGIN_H__

#include <libpeas/peas.h>

#include "../../../testing-util/peas-test-common.h"

G_BEGIN_DECLS

#define TESTING_TYPE_EMBEDDED_PLUGIN         (testing_embedded_plugin_get_type ())
#define TESTING_EMBEDDED_PLUGIN(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TESTING_TYPE_EMBEDDED_PLUGIN, TestingEmbeddedPlugin))
#define TESTING_EMBEDDED_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TESTING_TYPE_EMBEDDED_PLUGIN, TestingEmbeddedPlugin))
#define TESTING_IS_EMBEDDED_PLUGIN(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TESTING_TYPE_EMBEDDED_PLUGIN))
#define TESTING_IS_EMBEDDED_PLUGIN_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TESTING_TYPE_EMBEDDED_PLUGIN))
#define TESTING_EMBEDDED_PLUGIN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TESTING_TYPE_EMBEDDED_PLUGIN, TestingEmbeddedPluginClass))

typedef struct _TestingEmbeddedPlugin         TestingEmbeddedPlugin;
typedef struct _TestingEmbeddedPluginClass    TestingEmbeddedPluginClass;

struct _TestingEmbeddedPlugin {
  PeasExtensionBase parent_instance;
};

struct _TestingEmbeddedPluginClass {
  PeasExtensionBaseClass parent_class;
};

PEAS_TEST_EXPORT
GType                testing_embedded_plugin_get_type       (void) G_GNUC_CONST;
G_MODULE_EXPORT void testing_embedded_plugin_register_types (PeasObjectModule *module);

G_END_DECLS

#endif /* __TESTING_EMBEDDED_PLUGIN_H__ */
