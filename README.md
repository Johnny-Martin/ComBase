# ComBase
Hello COM!
df

DirectUIHWND

DuiEngine 	       				 http://code.taobao.org/svn/duiengine/trunk
duilib    	       				 https://github.com/duilib/duilib/trunk
LuaCom(MIT license)    	   		 http://github.com/davidm/luacom/trunk
libpng(Libpng license)    	     https://github.com/glennrp/libpng/trunk
libpng15                         https://github.com/glennrp/libpng/branches/libpng15
freetype(FreeType license)       http://sourceforge.net/projects/freetype/files/freetype2/2.6.1/    download zip
TinyXml2(ZLib license)  	     https://github.com/leethomason/tinyxml2/trunk
Lua(MIT license)  	      		 http://www.lua.org/ftp/lua-5.3.1.tar.gz   website：http://www.lua.org/versions.html#5.3
zlib(ZLib license)               https://github.com/madler/zlib/trunk
gaclib(Apache License)(by vczh)  https://github.com/vczh-libraries/Release/trunk
ZLog                             https://github.com/lopsd07/WinZlog/trunk
TinyBind                         http://nchc.dl.sourceforge.net/project/tinybind/tinybind/0.1/tinybind.tar.gz
NInvole                          https://github.com/lxrite/NInvoke/trunk

Win7 SDK                         https://www.microsoft.com/en-us/download/details.aspx?id=8442 
(GRMSDK_EN_DVD.iso for X86; GRMSDKIAI_EN_DVD.iso for Itanium; GRMSDKX_EN_DVD.iso for X64)
(if install failed, please uninstall all Visual C++ 2010 Runtimes from your machine (both x86 and x64),
then run the iso file, *UNCHECK* the Visual C++ Compilers option when selecting which options to install)

*NOTE* if you use VS2008,and you want develop with Windows SDK 7.1 , you must change the environment variable
"WindowsSdkDir", it's default value in VS2008 is "C:\Program Files\Microsoft SDKs\Windows\v6.0A", you can get the
value in VS2008: tool->visual stdio 2008 command prompt, input "set" and enter, find the "WindowsSdkDir"

How to change WindowsSdkDir's value to SDK7.1:
After SDK7.1 was installed, click: start->all apps->Microsoft Windows SDK V7.1->Windows SDK Configuration Tool
then select 7.1 in the drop list, Restart VS2008 at the end.
