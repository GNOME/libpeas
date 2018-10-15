# Determine whether we build the lua loader

!if "$(LUA)" == ""
LUA = lua
!endif

!if [$(LUA) -e "lgi=require 'lgi';glib=lgi.require('GLib')" 2> testlua.xx]
!endif

!if [for %l in (testlua.xx) do @(echo LGI_ERR=%~zl > luaconf.mak) & @(del %l)]
!endif

!if [$(LUA) -e "c=io.open(\"luaconf.mak\", \"a+\");ver, count=string.gsub(string.match(_VERSION, \"%d.%d\"), \"%.\", \"\");io.output(c);io.write(\"LUAVER=\" .. ver);io.close(c)"]
!endif

!include luaconf.mak

!if [del /f /q luaconf.mak]
!endif