/*
 * extension-gjs.js
 * This file is part of libpeas
 *
 * Copyright 2023 Christian Hergert
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

import Gio from 'gi://Gio';
import GObject from 'gi://GObject';
import Introspection from 'gi://Introspection';
import Peas from 'gi://Peas';

export let ExtensionGJSAbstract = GObject.registerClass({
    GTypeName: 'ExtensionGJSAbstract',
}, class ExtensionGJSAbstract extends Introspection.Abstract {});

export let ExtensionGJSPlugin = GObject.registerClass({
    GTypeName: 'ExtensionGJSPlugin',
    Implements: [
        Introspection.Callable,
        Introspection.Activatable,
        Introspection.Base,
        Introspection.HasPrerequisite,
    ],
    Properties: {
        'object': GObject.ParamSpec.object('object', null, null, GObject.ParamFlags.READWRITE, GObject.Object),
        'plugin-info': GObject.ParamSpec.object('plugin-info', null, null, GObject.ParamFlags.READWRITE, Peas.PluginInfo),
        'update-count': GObject.ParamSpec.int('update-count', null, null, GObject.ParamFlags.READABLE, 0, 1000000, 0),
    },
}, class ExtensionGJSPlugin extends Introspection.Prerequisite {

        get update_count() {
            if (this._update_count === undefined)
                this._update_count = 0;
            return this._update_count;
        }

        vfunc_activate() {
            this._update_count += 1;
        }

        vfunc_get_plugin_info() {
            return this.plugin_info;
        }

        vfunc_get_settings() {
            return this.plugin_info.get_settings(null);
        }

        vfunc_call_with_return() {
            return "Hello, World!";
        }

        vfunc_call_single_arg() {
            return true;
        }

        vfunc_call_multi_args(in_, inout) {
            return [inout, in_];
        }
    }
);
