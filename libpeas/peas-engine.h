/*
 * peas-engine.h
 * This file is part of libpeas
 *
 * Copyright (C) 2002-2005 - Paolo Maggi
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

#ifndef __PEAS_ENGINE_H__
#define __PEAS_ENGINE_H__

#include <glib.h>

#include "peas-plugin-info.h"
#include "peas-extension.h"
#include "peas-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_ENGINE              (peas_engine_get_type ())
#define PEAS_ENGINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), PEAS_TYPE_ENGINE, PeasEngine))
#define PEAS_ENGINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), PEAS_TYPE_ENGINE, PeasEngineClass))
#define PEAS_IS_ENGINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), PEAS_TYPE_ENGINE))
#define PEAS_IS_ENGINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_ENGINE))
#define PEAS_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_TYPE_ENGINE, PeasEngineClass))

typedef struct _PeasEngine         PeasEngine;
typedef struct _PeasEngineClass    PeasEngineClass;
typedef struct _PeasEnginePrivate  PeasEnginePrivate;

/**
 * PeasEngine:
 *
 * The #PeasEngine structure contains only private data and should only be
 * accessed using the provided API.
 */
struct _PeasEngine {
  GObject parent;

  /*< private > */
  PeasEnginePrivate *priv;
};

/**
 * PeasEngineClass:
 * @parent_class: The parent class.
 * @load_plugin: Signal class handler for the #PeasEngine::load-plugin signal.
 * @unload_plugin: Signal class handler for the #PeasEngine::unload-plugin signal.
 *
 * Class structure for #PeasEngine.
 */
struct _PeasEngineClass {
  GObjectClass parent_class;

  void     (*load_plugin)                 (PeasEngine     *engine,
                                           PeasPluginInfo *info);

  void     (*unload_plugin)               (PeasEngine     *engine,
                                           PeasPluginInfo *info);

  /*< private >*/
  gpointer padding[8];
};

PEAS_AVAILABLE_IN_ALL
GType             peas_engine_get_type            (void) G_GNUC_CONST;
PEAS_AVAILABLE_IN_ALL
PeasEngine       *peas_engine_new                 (void);
PEAS_AVAILABLE_IN_ALL
PeasEngine       *peas_engine_new_with_nonglobal_loaders
                                                  (void);
PEAS_AVAILABLE_IN_ALL
PeasEngine       *peas_engine_get_default         (void);

PEAS_AVAILABLE_IN_ALL
void              peas_engine_add_search_path     (PeasEngine      *engine,
                                                   const gchar     *module_dir,
                                                   const gchar     *data_dir);
PEAS_AVAILABLE_IN_ALL
void              peas_engine_prepend_search_path (PeasEngine      *engine,
                                                   const gchar     *module_dir,
                                                   const gchar     *data_dir);

/* plugin management */
PEAS_AVAILABLE_IN_ALL
void              peas_engine_enable_loader       (PeasEngine      *engine,
                                                   const gchar     *loader_name);
PEAS_AVAILABLE_IN_ALL
void              peas_engine_rescan_plugins      (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
const GList      *peas_engine_get_plugin_list     (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
gchar           **peas_engine_get_loaded_plugins  (PeasEngine      *engine);
PEAS_AVAILABLE_IN_ALL
void              peas_engine_set_loaded_plugins  (PeasEngine      *engine,
                                                   const gchar    **plugin_names);
PEAS_AVAILABLE_IN_ALL
PeasPluginInfo   *peas_engine_get_plugin_info     (PeasEngine      *engine,
                                                   const gchar     *plugin_name);

/* plugin loading and unloading */
PEAS_AVAILABLE_IN_ALL
gboolean          peas_engine_load_plugin         (PeasEngine      *engine,
                                                   PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean          peas_engine_unload_plugin       (PeasEngine      *engine,
                                                   PeasPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
void              peas_engine_garbage_collect     (PeasEngine      *engine);

PEAS_AVAILABLE_IN_ALL
gboolean          peas_engine_provides_extension  (PeasEngine      *engine,
                                                   PeasPluginInfo  *info,
                                                   GType            extension_type);


G_GNUC_BEGIN_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
PeasExtension    *peas_engine_create_extensionv   (PeasEngine      *engine,
                                                   PeasPluginInfo  *info,
                                                   GType            extension_type,
                                                   guint            n_parameters,
                                                   GParameter      *parameters);
G_GNUC_END_IGNORE_DEPRECATIONS

PEAS_AVAILABLE_IN_1_24
PeasExtension    *peas_engine_create_extension_with_properties
                                                  (PeasEngine     *engine,
                                                   PeasPluginInfo *info,
                                                   GType           extension_type,
                                                   guint           n_properties,
                                                   const gchar   **prop_names,
                                                   const GValue   *prop_values);

PEAS_AVAILABLE_IN_ALL
PeasExtension    *peas_engine_create_extension_valist
                                                  (PeasEngine      *engine,
                                                   PeasPluginInfo  *info,
                                                   GType            extension_type,
                                                   const gchar     *first_property,
                                                   va_list          var_args);
PEAS_AVAILABLE_IN_ALL
PeasExtension    *peas_engine_create_extension    (PeasEngine      *engine,
                                                   PeasPluginInfo  *info,
                                                   GType            extension_type,
                                                   const gchar     *first_property,
                                                   ...);


G_END_DECLS

#endif /* __PEAS_ENGINE_H__ */
