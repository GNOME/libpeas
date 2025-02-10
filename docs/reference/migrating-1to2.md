Title: Migrating from Peas 1.x to Peas 2
Slug: peas-migrating-1-to-2

Peas 2 is a major new version of Peas that breaks both API and ABI compared to
Peas 1.0. Changes to the API, such as removal of the GTK interfaces and
adoption of `GListModel`, make it easier to adapt to broader changes in the
platform and modern UX patterns.

After migrating to Peas 2, applications can take advantage of the
[new list widgets][gtk4-listwidgets] if using GTK4, or continue using GTK 3
with the existing widgets like [`Gtk.ListBox`][gtk3-listbox].

## Overview

The steps outlined in the following sections assume that your application
is being built with GLib 2.74 or greater. If you are using an older version
of GLib, you should first ensure that your application is updated to build
with the latest minor release in the 2.74 series.

#### Summary of Changes

* Build your application against `glib-2.0 >= 2.74`
* The pkg-config name is `libpeas-2` and the GI name is `Peas-2`
* Replace `#include <libpeas/peas.h>` with `#include <libpeas.h>`
* Replace the `PeasExtension` alias with `GObject`
* Replace `PeasActivatable` and `PeasGtkConfigurable` with custom interfaces
* `PeasEngine` and `PeasExtensionSet` implement `GListModel`
* `PeasPluginInfo` is now a `GObject`

#### Removed Symbols

* `PeasActivatable`
* `PeasExtension`
* `PeasGtkConfigurable`
* `PeasGtkPluginManager`
* `PeasGtkPluginManagerView`
* `peas_engine_create_extensionv()`
* `peas_engine_get_loaded_plugins()`
* `peas_engine_prepend_search_path()`
* `peas_extension_set_call()`
* `peas_extension_set_call_valist()`
* `peas_extension_set_callv()`

## Headers and Imports

The C header file has been renamed to `libpeas.h` (from `libpeas/peas.h`), which
can be safely changed with a search-replace function.

```diff
- #include <libpeas/peas.h>
+ #include <libpeas.h>
```

The `pkg-config` name for Peas 2 is `libpeas-2`, while the GI name is now
`Peas-2` (used in GIR dependencies, gi-docgen, and so on). For GJS, Python, Vala
and other language bindings, the namespace remains `Peas`.

## Removed Interfaces and Features

As of Peas 2, interfaces and classes that might have been marked for
deprecation have been removed entirely, allowing for a minimal public API.

Additionally, support for Python 2 plugins has been dropped, while
support for writing extensions in GJS has been added. See the
[Plugin Loaders](#plugin-loaders) section for more information.

### PeasActivatable

Interfaces and abstract base-classes are easy to define, and developers are
encouraged to create more purposeful extension points for their application.

### PeasExtension

The `Peas.Extension` alias for `GObject.Object` has been removed, and all APIs
that used it will use `GObject.Object` instead. This change can usually be made
with a simple search-replace operation in a project.

### PeasGtkConfigurable, PeasGtkPluginManager, PeasGtkPluginManagerView

The GTK 3 interfaces and widgets have been removed, which also means that
libpeas has no dependency on any particular UI toolkit. As with
`PeasActivatable`, developers are encourage to define interfaces suited to
their use case and take advantage of the new list widgets in GTK 4.

## Updated Interfaces and Features

Peas has been updated to take advantage of changes in GTK 4, especially
the emphasis on `GListModel` in user interface patterns.

### PeasPluginInfo

[class@Peas.PluginInfo] is now a GObject, instead of a boxed
type, making it possible to bind properties and integrate with APIs that
operate on GObjects. It also means you may hold a reference to a plugin info
if necessary, instead of creating a copy of the structure.

Any GObject properties defined with `g_param_spec_boxed()` must be replaced
with `g_param_spec_object()`, while `GObject.Object.get_property()` and
`GObject.Object.set_property()` should use `g_value_set_object()` and
`g_value_get_object()` (or `g_value_dup_object()`), respectively.

Additionally, this change may require replacing calls to `g_boxed_copy()` and
`g_boxed_free()` with `g_object_ref()` and `g_object_unref()`, but also
allows the use of `g_autoptr()` for compilers that support it.

### PeasEngine

[class@Peas.Engine] is now a [iface@Gio.ListModel] of
[class@Peas.PluginInfo] objects. This makes it simple to bind properties to labels
and other widgets within rows of a [class@Gtk.ListView] and filter lists using
type-to-search with [class@Gtk.FilterListModel].

The `peas_engine_prepend_search_path()` method has been removed, due to
confusing semantics, and `peas_engine_add_search_path()` should be used instead.

The `peas_engine_create_extensionv()` method has been removed, due to
deprecation of `GParameter`, and applications should instead call one of:

* `peas_engine_create_extension()`
* `peas_engine_create_extension_valist()`
* `peas_engine_create_extension_with_properties()`

### PeasExtensionSet

[class@Peas.ExtensionSet] also implements [iface@Gio.ListModel] which
improves situations where an application wants to limit which extensions within
a set are active based on arbitrary conditions.

The following methods have been removed, and applications should instead
invoke methods on an extension directly:

* `peas_extension_set_call()`
* `peas_extension_set_call_valist()`
* `peas_extension_set_callv()`

## Plugin Loaders

### GJS

New in Peas 2 is support for plugins written in [GJS][gjs], GNOME's JavaScript
bindings for the platform. GJS is well maintained and receives considerable
testing as a part of GNOME Shell and other core applications.

Despite this, using GJS as plugin loader will require more real-world
testing and you are encouraged to report any issues you encounter in the
[Peas issue tracker][peas-issues] and [GJS issue tracker][gjs-issues],
respectively.

### Lua

The Lua plugin loader remains unchanged, however the status of the language
bindings is dependent on the [upstream project][lgi].

### Python

Python 3 has been widely adopted and [PyGObject][pygobject], provides the
actively maintained language bindings for the platform.

Accordingly, the Python 2 plugin loader has been removed and any applications
depending on it should continue to use Peas 1.0, or migrate to Python 3.

In Libpeas 2 the "python3" loader has been renamed to "python".

## GTK 4 Examples

Below are a few simple examples of how applications can benefit from and
employ the new list widgets in GTK 4.

### PeasEngine and GtkListView

```xml
<object class="GtkListView">
  <property name="model">
    <object class="GtkNoSelection">
      <property name="model">
        <!-- PeasEngine goes here -->
      </property>
    </object>
  </property>
  <property name="factory">
    <object class="GtkBuilderListItemFactory">
      <property name="bytes">
<![CDATA[
  <?xml version="1.0" encoding="UTF-8"?>
  <interface>
    <template class="GtkListItem">
      <property name="child">
        <object class="GtkBox">
          <property name="spacing">12</property>
          <child>
            <object class="GtkImage">
              <binding name="icon-name">
                <lookup name="icon-name" type="PeasPluginInfo">
                  <lookup name="item">GtkListItem</lookup>
                </lookup>
              </binding>
            </object>
          </child>
          <child>
            <object class="GtkLabel">
              <binding name="label">
                <lookup name="name" type="PeasPluginInfo">
                  <lookup name="item">GtkListItem</lookup>
                </lookup>
              </binding>
            </object>
          </child>
        </object>
      </property>
    </template>
  </interface>
]]>
      </property>
    </object>
  </property>
</object>
```


[gtk3-listbox]: https://docs.gtk.org/gtk3/class.ListBox.html
[gtk4-listwidgets]: https://docs.gtk.org/gtk4/section-list-widget.html
[glistmodel]: https://docs.gtk.org/gio/iface.ListModel.html
[peas-engine]: https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-2/class.Engine.html
[peas-extensionset]: https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-2/class.ExtensionSet.html
[peas-plugininfo]: https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-2/class.PluginInfo.html
[gjs-issues]: https://gitlab.gnome.org/GNOME/gjs/issues
[peas-issues]: https://gitlab.gnome.org/GNOME/libpeas/issues
[gjs]: https://gitlab.gnome.org/GNOME/gjs
[pygobject]: https://gitlab.gnome.org/GNOME/pygobject
[lgi]: https://github.com/lgi-devs/lgi
