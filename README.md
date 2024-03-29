# Introducing libpeas

libpeas is a gobject-based plugins engine, and is targetted at giving every
application the chance to assume its own extensibility. It is currently used by
several Gnome applications like gedit and Totem.

It takes its roots in the old gedit plugins engine, and provides an extensive set
of features mirroring the desiderata of most of the applications providing an
extension framework.

# Documentation

Documentation can be found at:

 * Peas-2: https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-2/

See the [Migration Guide](https://gnome.pages.gitlab.gnome.org/libpeas/libpeas-2/migrating-1to2.html)
for help migrating from Peas-1.0 to Peas-2.

# Multiple extension points

One of the most frustrating limitations of the Gedit plugins engine was that it
only allows extending a single class, called GeditPlugin. With libpeas, this
limitation vanishes, and the application writer is now able to provide a set of
GInterfaces the plugin writer will be able to implement as his plugin requires.

# On-demand programming language support

libpeas comes with a set of supported languages (currently, C, Lua 5.1,
and Python 3). Those languages are supported through “loaders” which
are loaded on demand. What it means is that you only pay for what you use: if
you have no Python plugin, the Python interpreter won't be loaded in memory.
Of course, the same goes for the C loader.

# Damn simple to use (or at least we try hard)

Adding support for libpeas-enabled plugins in your own application is a matter
of minutes. You only have to create an instance of the plugins engine, and
call methods on the implementations of the various extension points. That's it,
no clock harmed.

# A shared library for everyone

As I noted earlier, the latest improvements of our beloved development platform
made it possible to create bindings for apps very quickly. And with the Gnome 3
announcement, this looked like the perfect timing to make the plugins engine
and its latest improvements available to everyone, with a library. Also,
hopefully it will reduce code duplication and allow bugs to be fixed at the
right place, once and for all, improving the quality of our applications.

As a member of the Gnome community and as an offspring of gedit, libpeas already
shares most of the Gnome infrastructure and philosophy:

    * You can download the first release tarball on the Gnome FTP server.
    * You can browse the source and contribute using our git repository.
    * You can come and discuss with me and others on #libpeas (GimpNet)
    * And you can report bug or propose new features through the good old Gnome
      Bugzilla, against the libpeas module.

# A few hints on using libpeas

As always for the new projects, it can take some time to grasp all the
subtleties at first.

## Plugins versus Extensions

Something that is going to puzzle most of the newcomers is the fact that the
libpeas API talks about both plugins and extensions, two terms that are usually
used interchangeably, but who have very different meanings in libpeas.

Let's try and give a definition of both of these words in this context:

 * Plugin: In the context of libpeas, a plugin is a logical package. It's what
   you will enable or disable from the UI, and at the end it is what the user
   will see. An example of plugin for gedit would be the file browser plugin.

 * Extension. An extension is an object which implements an interface
   associated to an extension point. There can be several extensions in a single
   plugin. Examples of extensions provided by the file browser plugin would be
   the configuration dialog, the left panel pane and a completion provider for
   file names.

Copied from http://log.istique.net/2010/announcing-libpeas
