btlauncher plugin using FireBreath

usage
see test/fblauncher_test.html


Torque or Soshare build process for Windows (Visual Sstudio 2008)
=================================================================
This will get you started.  Buildbot follows a similar process.
Once the "build" directory is created with Item 25 it is not necessary to repeat it.
Buildbot will not get any changes you put into the "build" directory since it always
invokes the script to create that directory.  The specificas of what FireBreath
constucts in the build directory are indicated by the *PluginConfig.cmake file
that is copied to PluginConfig.cmake in item 24.

The files in the top level of the btlauncher directory do not seem to be altered
by this process, so, editing and committing changes there appears to be safe.

Item	Process                                                |   Additional Info
----	-------                                                |   ---------------
1       git clone ut_win (or your own fork of ut_win)          |   git@github.com:bittorrent/ut_win.git
2       git clone ut_core                                      |   git@github.com:bittorrent/ut_core.git
3       git clone srp                                          |   git@github.com:bittorrent/srp.git
4       git clone btlauncher                                   |   git@github.com:bittorrent/btlauncher.git
5       git clone FireBreath (this is not a BitTorrent repo)   |   git@github.com:firebreath/FireBreath.git
6                                                              |
7       Install cmake (not the cygwin version)                 |   http://www.cmake.org/files/v2.8/cmake-2.8.8-win32-x86.exe
                                                               |
8       Install wix installer toolkit                          |   The trailing '\' that wix puts at the end of its WIX environment 
                                                               |   variable causes FireBreath to not add msi project to the solution.  
                                                               |   in the Windows cmd look at the "set" command to see what WIX= .  
                                                               |   If the trailing '\' is there, do set WIX=<what ever is there 
                                                               |   without the offending slash>
9                                                              |
10      In the FireBreath repository:  checkout firebreath-1.6 |	
11      in the ut_win repository:  checkout 3.3                |
12                                                             |
13                                                             |
14                                                             |
15                                                             |
16      In the "Bundle" directory of the btlauncher repository,|
        delete all executables                                 |
17      In the btlauncher repository, if there is a "build"    |
        directory, delete it.                                  |
18                                                             |
19      In the ut_win repository, launch visual studio         |   Alternativly, use buildbot to generate the execuables
        on winclients.sln                                      |
                                                               |
20      Select the SoShare/Torque build configuration to build |
        and build it/them	
21                                                             |
22      Copy the SoShare/Torque executables to the "Bundle"    |
        directory of the btlauncher repository                 |  Copy from either your local builds or buildbot products
23                                                             |
24      In the btlauncher repository, copy one of the          |
        following to "PluginConfig.cmake":                     |
                                                               |
        TorquePluginConfig.cmake                               |   What ever cmake script is in PluginConfig.cmake will be used by the 
        SharePluginConfig.cmake                                |   FireBreath script to generate the plugin files.
        ChromePluginConfig.cmake                               |
        LivePluginConfig.cmake                                 |
                                                               |
25      From within the btlauncher repository, execute the     |
        FireBreath prep2008.cmd with the argument of '.' to    |   "..\FireBreath\prep2008.cmd ."
        work in this directory (this requires using a          |
        windows cmd shell)                                     |   This script will create the ""build"" directory within the btlauncher repository"
                                                               |
26      Open VS2008 with "FireBreath.sln" in the "build"       |
        directory of the btlauncher repository                 |
                                                               |
27      Build the entire solution                              |   The .msi file is generated by the *_WiXInstall project in the 
                                                               |   solution.  If it is missing, see the additional info in item 8.
28      The .msi file is in                                    |
        btlauncher/build/bin/<*>/[Debug/Release] directory     |   


- If using Visual Studio Express, download WinDDK 7.1, and install the 'Build Environments' 
- http://www.microsoft.com/en-us/download/details.aspx?id=11800
- follow FireBreath docs to run prep2008.cmd on this project
- build generated visual studio project

debug instructions (windows):
- go to build\bin\SoShare\Debug folder and run "regsvr32.exe npSoShare.dll"
- open browser, use visual studio to attach to browser process (make sure you're attaching to "Native" and not "Script" code)

build instructions (osx):
- install [cmake](http://www.cmake.org/files/v2.8/cmake-2.8.8-Darwin64-universal.dmg)
- when in the btlauncher repo, run: ../FireBreath/prepmac.sh . build_mac
- If you have macports installed, cmake is likely to have found the incorrect libxml2.dylib, if
  so, open the resulting build_mac/FireBreath.xcodeproj/project.pbxproj and replace any occurrance
  of /opt/local/lib/libxml2.dylib with /usr/lib/libxml2.dylib

