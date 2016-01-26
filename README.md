pcbsd
=====

The official PC-BSD git repository

Source Map
---------------------------------

`build-files/`

Contains iso image configuration files, package settings, dummy port settings, and build settings.

<br>
`overlays/`
 
Files included on our install images, scripts, and role settings for the installer.

<br>
`src-qt5/`
 
C++ Code for pcbsd-utils-qt5 package, which builds into all the various PC-BSD related GUI utilities.

Note: Qt5 build tools are located in `/usr/local/lib/qt5/bin/` rather than `/usr/local/bin/`

*Requires Qt5 to build / run*

To create the Makefile in src-qt5, make sure `devel/qt5` is installed on your system and then run:
```% cd src-qt5 && /usr/local/lib/qt5/bin/qmake```

<br>
`pbi-modules/`

Contains PC-BSD's PBI .conf files.

<br>
`retired/`

Any PC-BSD utilities or files that are no longer in use.

<br>
`src-sh/`

Shell code which builds into pcbsd-utils package. Scripts, backends and various CLI
related utilities for TrueOS & PC-BSD. 

<br>
`src-webui/`

All the code for the AppCafe WEB interface.


Contacting Us
---------------------------------

######IRC (FreeNode)
---------------------------------
`#pcbsd-dev` (Developer questions / talk)

`#pcbsd` (General user information)

<br>
######Mailing Lists
---------------------------------
http://lists.pcbsd.org

<br>
######Bug Tracker
---------------------------------
http://bugs.pcbsd.org
