pcbsd
=====

The official PC-BSD git repository

Source Map
---------------------------------

build-files/
  Contains iso image configuration files, package settings, dummy port settings, and build settings.

overlays/
  Files included on our install images, scripts, and role settings for the installer.

src-qt4/
  C++ Code for pcbsd-utils-qt4 package, which builds into all the various PC-BSD related
  GUI utilities.
  
  Requires QT4 to build / run.
  
pbi-modules/
  Contains PC-BSD's PBI .conf files.
  
retired/
  Any PC-BSD utilities or files that are no longer in use.

src-sh/
  Shell code which builds into pcbsd-utils package. Scripts, backends and various CLI
  related utilities for TrueOS & PC-BSD. 

src-webui
  All the code for the AppCafe WEB interface.
