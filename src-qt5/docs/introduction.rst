.. include global.rst

**Preface** 

Written by users of the PC-BSD® operating system.

Version |version|

Copyright © 2005 - 2015 The PC-BSD® Project.

Welcome to PC-BSD®! This Handbook covers the installation and use of PC-BSD® |version|. This Handbook is a work in progress and relies on the contributions of many individuals. If you are interested in assisting with the Handbook, refer to the documentation `README <https://github.com/pcbsd/pcbsd/blob/master/src-qt5/docs/README.md>`_. If you use IRC Freenode, you are welcome to join the #pcbsd channel where you will find other PC-BSD® users.

Previous versions of the Handbook in various formats and languages are available from `here <ftp://ftp.pcbsd.org/pub/handbook/>`_. 

The PC-BSD® Users Handbook is freely available for sharing and redistribution under the terms of the `Creative Commons Attribution License <https://creativecommons.org/licenses/by/4.0/>`_. This means that you have permission to copy, distribute, translate, and adapt the work as long as you attribute the PC-BSD® Project as the original source of the Handbook.

PC-BSD® and the PC-BSD® logo are registered trademarks of `iXsystems <https://www.ixsystems.com/>`_. If you wish to use the PC-BSD® logo in your own works, ask for permission first from marketing@ixsystems.com.

AMD is a trademark of Advanced Micro Devices, Inc.

Apache is a trademark of The Apache Software Foundation.

AppCafe® is a registered trademark of `iXsystems <https://www.ixsystems.com/>`_.

Asus® and Eee PC® are registered trademarks of ASUSTeK® Computer Inc.

Facebook® is a registered trademark of Facebook Inc.

Flash® is a registered trademark of Adobe Systems Incorporated in the United States and/or other countries.

FreeBSD® is a registered trademark of the `FreeBSD Foundation <https://www.freebsdfoundation.org/>`_. 

FreeNAS® is a registered trademark of `iXsystems <https://www.ixsystems.com/>`_.

IBM® is a registered trademark of International Business Machines Corporation.

Intel, the Intel logo, Pentium Inside, and Pentium are trademarks of Intel Corporation in the U.S. and/or other countries.

Java™ is a trademark of Oracle America and/or its affiliates in the United States and other countries.

Lenovo® is a registered trademark of Lenovo.

LinkedIn® is a registered trademark of LinkedIn Corporation.

Linux® is a registered trademark of Linus Torvalds.

Mac and Mac OS are trademarks of Apple Inc., registered in the U.S. and other countries.

MacBook® is a registered trademark of Apple Inc.

MySQL is a trademark of Oracle.

NVIDIA® is a trademark and/or registered trademark of NVIDIA Corporation in the U.S. and other countries.

PostgreSQL® is a registered trademark of the PostgreSQL Global Development Group.

ThinkPad® is a registered trademark of Lenovo.

TrueOS® is a registered trademark of `iXsystems <https://www.ixsystems.com/>`_.

Twitter is a trademark of Twitter, Inc. in the United States and other countries.

UNIX® is a registered trademark of The Open Group.

VirtualBox® is a registered trademark of Oracle.

VMWare® is a registered trademark of VMWare, Inc.

Warden® is a registered trademark of `iXsystems <https://www.ixsystems.com/>`_.

Windows® is a registered trademark of Microsoft Corporation in the United States and other countries.

**Typographic Conventions** 

* Names of graphical elements such as buttons, icons, fields, columns, and boxes are enclosed within quotes. For example: click the "Browse Categories" button.

* Menu selections are italicized and separated by arrows. For example: :menuselection:`Control Panel --> About`.

* Commands that are mentioned within text are highlighted in :command:`bold text`. Command examples and command output are contained in green code blocks.

* File names are enclosed in a blue box :file:`/like/this`.

* Keystrokes are formatted in a blue box. For example: press :kbd:`Enter`.

* **bold text** is used to emphasize an important point.

* *italic text* is used to represent device names or text that is input into a GUI field.

Introduction
************

Welcome to PC-BSD®!

`PC-BSD® <http://www.pcbsd.org/>`_ began in 2005 when Kris Moore presented the first beta version of a FreeBSD operating system pre-configured for desktop use. Since then, PC-BSD® has matured into a polished, feature-rich, free-of-charge, open source operating system that meets the desktop or server needs of the beginner to the advanced user alike.

PC-BSD® is essentially a customized installation of FreeBSD, not a forked derivative. Since the underlying FreeBSD system has been kept intact, you have a fully functional FreeBSD system under the hood. PC-BSD® provides a graphical installer which can be used to easily install a desktop or a server version of FreeBSD, known as TrueOS®. Other differences from FreeBSD include: 

* PC-BSD® pre-configures at least one desktop environment during a desktop installation. Installed desktop environments appear in the login menu, allowing the user to select which environment to log into.

* The PC-BSD® graphical installer supports additional features such as configuring ZFS and encryption during installation.

* PC-BSD® provides a graphical software management system for the desktop and a command line equivalent for the server.

* PC-BSD® provides a :ref:`Control Panel` of utilities for configuring the system. The graphical versions of these utilities are available on both versions, the desktop and server.

* PC-BSD® comes pre-configured with a number of automatic scripts to perform tasks such as connecting digital cameras or USB memory sticks.

* The PC-BSD® boot menu supports boot environments, or snapshots of the operating system, and the PC-BSD® Update Manager automatically adds a new boot environment to the boot menu before updating the operating system or software. This means that if an update fails, you can reboot into the previous version of the operating system, before the update occurred.

PC-BSD® started off as an independent project, but since October, 2006 PC-BSD® is financially backed and supported by the enterprise-class hardware solutions provider `iXsystems <https://www.ixsystems.com/>`_.

.. index:: features
.. _Goals and Features:

Goals and Features
==================

PC-BSD® provides the following features: 

* **Easy installation:** to install either a graphical desktop or command-line server version of PC-BSD®, simply insert the installation media, reboot the
  system to start the installer, and answer a few questions in the installation menus.

* **Automatically configured hardware:** video, sound, network, and other devices are automatically configured for you.

* **Intuitive desktop interface:** PC-BSD® comes with a choice of :ref:`Desktops` to support your day-to-day computing needs.

* **Easy software management:** with :ref:`AppCafe®`, installing, upgrading, and uninstalling software is safe and easy.

* **Lots of software available:** in addition to its own software, PC-BSD® can install software that has been ported to FreeBSD (currently over 24,700
  applications).

* **Easy to update:** PC-BSD® provides a built-in :ref:`Update Manager` that will notify you of available updates and allow you to apply operating system
  security fixes, bug fixes, and system enhancements as well as upgrade to newer versions of the operating system or installed software.

* **Virus-free:** PC-BSD® is not affected by viruses, spyware, or other malware.

* **No defragmentation:** PC-BSD® hard drives do not need to be defragmented and do not slow down over time. PC-BSD® uses OpenZFS which is a self-healing
  filesystem.

* **Laptop support:** provides power saving and swap space encryption and automatically switches between wired and wifi network connections.

* **Secure environment:** PC-BSD® provides a pre-configured firewall and a built-in host-based Intrusion Detection System.

* **Easy system administration:** PC-BSD® provides a :ref:`Control Panel` containing many graphical tools for performing system administration tasks.

* **Localization:** PC-BSD® supports a number of native languages and locales.

* **Vibrant community:** PC-BSD® has a friendly and helpful community. 

* **Professional support:** professional email and phone support is available from
  `iXsystems <https://www.ixsystems.com/support/>`_.

.. index:: What's New
.. _What's New in |version|:

What's New in |version|
=======================

The following features or enhancements were introduced for PC-BSD® |version|:

* PC-BSD® packages are now built with `LibreSSL <http://www.libressl.org/>`_ which has fewer vulnerabilities than OpenSSL.

* `OpenNTPD <http://www.openntpd.org/>`_ has replaced the default NTP as it has fewer security vulnerabilities.

* Lumina has been updated to 0.8.4-Release-P1.

* The ability to login as a :ref:`PersonaCrypt` user has been added. This allows a user to use a removable, encrypted device as their home directory.

* The ability to log into a "stealth session" has been added. A stealth session creates an encrypted zvol as a temporary home directory for that login session.
  When the user logs out of a stealth session, the zvol is destroyed, along with the contents of the temporary home directory. 

* :ref:`Tor Mode` has been added, making it possible to anonymously access Internet sites as this mode automatically forwards all Internet traffic through the
  `Tor Project's <https://www.torproject.org/>`_ transparent proxy service.

* The **pc-webkitviewer** command line utility has been added for viewing URLs or local :file:`.html` files. A desktop shortcut to this utility is provided for displaying
  the HTML version of this Handbook. Run this command without any options to view its usage.

* The **pc-pdfviewer** command line utility has been added for viewing PDF files. It includes a presentation mode for displaying the PDF as a presentation. Press
  :kbd:`Esc` to leave a presentation or press :kbd:`F11` to start or end a presentation. The arrow keys can be used to navigate the presentation and the :kbd:`Home` and
  :kbd:`End` keys can be used to skip to the first or last page of the PDF, whether or not it is in presentation mode. If printing is configured, this utility can be used to
  print or provide a print preview of the PDF.

* The source for the PC-BSD® documentation and its translations has been moved from the documentation wiki to the
  `PC-BSD® github repository <https://github.com/pcbsd/pcbsd/tree/master/src-qt5/docs>`_. This means that
  the documentation and its translations can be built with the operating system and documentation updates can be installed using :ref:`Update Manager`.

* The `zsh shell <http://www.zsh.org/>`_ has been added to the base system and is available for setting as a user's default shell in :ref:`User Manager`.

* :ref:`Kodi` and :ref:`PlexHome Theater` can be installed from the MediaCenter category of the :ref:`System Selection Screen` of the installer.

* The options in the :ref:`Advanced Mode` screen of the installer have been streamlined.

* The :menuselection:`utility --> zimport` menu of the text installer now lets you mount and access a specified boot environment in order to perform maintenance.

* The ability to create a :ref:`PersonaCrypt` user and to specify a UID has been added to the :ref:`Create a User Screen`.

* The ability to :ref:`Configure Audio Output` has been added as a post-installation configuration screen.

* The "Lock" and "Suspend" buttons have been added to the Lumina "Log Out" screen.

* A "Settings" menu has been added to the right-click menu of the Lumina desktop.

* The :ref:`Lumina Configuration Utility` has been reorganized.

* The :ref:`Lumina Search` and :ref:`Lumina Xconfig` utilities have been added.

* The new AppCafe®, formerly called AppCafe® Remote, has replaced the old version of AppCafe®.

* The :menuselection:`View --> Vulnerabilities` and :menuselection:`View --> Base updates history` have been added to :ref:`Update Manager`.

* An end-of-life notice has been added to the output of the :command:`pc-updatemanager` command to assist the administrator in making upgrade decisions.

* To save clutter in the boot menu when multiple boot environments exist, only the default boot environment and a link to the "Boot Environment Menu" appear. Click the
  "Boot Environment Menu" entry to see the complete list of available boot entries. Figures 8.4b and 8.4c provide example screenshots.

* The "Allow Stealth Sessions" checkbox has been added to :menuselection:`Control Panel --> Login Manager --> Misc`.

* The "UID" selection field has been added to :menuselection:`Control Panel --> User Manager --> Add User`.

* :ref:`PersonaCrypt` devices can be intialized and managed from :menuselection:`Control Panel --> User Manager --> Advanced Mode`.

* :ref:`Mount Tray` will automatically detect ZFS-formatted removable drives, providing the ability to import or export the ZFS pools from those devices when prompted, just
  like any other removable device.

* If the system has multiple audio outputs, the :ref:`PC-BSD Mixer Tray` will also display an "Output" menu for quickly changing the default audio output device.

* The **list-audiodev**,
  **usingtormode**,
  **setdefaultaudiodevice**, and
  **setscreenbrightness** options have been added to :ref:`pc-sysconfig`.

* IPFW is now the default firewall as it provides support for VIMAGE. :ref:`Firewall Manager` has been redesigned to use IPFW and its UI has been simplified to make it easier to use.

* The "Scrub schedule" screen has been added to the :ref:`Life Preserver` setup wizard and the Life Preserver "Configure" screen.

* The "Enable Offsite Backups" option has been added to the "File" menu of Life Preserver. This provides a wizard for configuring backups which are stored encrypted on a remote
  system.

* :ref:`Life Preserver` now does per-dataset replication rather than recursive replication. This change allows dataset exclusions and prepares the utility for
  resumable ZFS send/receive, once the FreeBSD version of OpenZFS supports this feature. This also makes replication more fault-tolerant, as it can restart from the specific dataset
  which was halted.

* The "Re-Initialize Replications" option has been added to the "Snapshots" menu of Life Preserver.

* The :command:`lpreserver-host-iscsi` script has been added for configuring :ref:`Replicating Encrypted Backups`. This new functionality provides an extra measure of security
  to replicated backups by adding support for fully-encrypted backups, using stunnel and GELI-backed iSCSI volumes. This means that the data stored on the remote side is
  encrypted and only accessibly with the key file stored on the PC-BSD® client.

* The ability for :ref:`Restoring the Operating System` from an encrypted backup has been added.

* The "AppCafe" and "Check for Updates" buttons have been removed from the "Tools" tab of :ref:`Warden®` as AppCafe® is used for :ref:`Managing Software in Jails`
  and jail updates are managed using :ref:`Update Manager`.

.. index:: What's New
.. _What's New Since |version|:

What's New Since |version|
==========================

Users who have configured their PC-BSD® system to use the "Edge" repository using the instructions in :ref:`Configuring AppCafe®` and who have not disabled automatic
updates as described in :ref:`How PC-BSD® Updating Works`, will automatically receive new features when they become available as an update. Some updates affect the user
interface so this section lists any functional changes that have occurred since |version|. As these new features are documented, the installed version of this Handbook is
automatically updated.

* CD-sized network installers are available for both the graphical and text installers. The installation looks the same for both, with the difference being the size of the
  downloaded media and the fact that the installation files are retrieved over the network rather than from the installation media.

* Lumina has been updated to 0.8.5.

* `iocage <https://github.com/iocage/iocage>`_ has replaced Warden as the back-end for jail management in :ref:`AppCafe®` and the "Warden" tab has been renamed to "Jails".

* The default Serif/Sans Serif font is now `Noto <http://www.google.com/get/noto/>`_ instead of Dejavu.

* The graphical installer now uses the `Droid <http://www.droidfonts.com/>`_ font.

* The "Enable Optional Services" screen has been added to the post-configuration wizard. Currently, this screen allows you to enable the SSH service which will automatically
  start this service and generate the firewall rules needed to allow SSH connections to the system.

* The "Tile Plugins" and "Cascade Plugins" entries have been added to the :ref:`Right-Click Menu` of Lumina.

* You can now create and manage more than two panels using the :ref:`Lumina Configuration Utility`.

* The "Enterprise (Long Term Support)" repository has been added to :menuselection:`AppCafe® --> Configure --> Repository Configuration`. This option is meant for
  enterprise users that wish to only receive software updates which fix known security vulnerabilities.

* The :menuselection:`System -> Branches` menu has been added to :ref:`Update Manager`. This can be used to change which software branch is used to track updates.

* The **showeol** option has been added to :command:`pc-updatemanager`.

* The "Allow Valid Users with UID under 1000" checkbox and "Additonal Excluded Users" field have been added to :menuselection:`Control Panel --> Login Manager --> Misc`.

* The **probe-netdrives**,
  **list-mountednetdrives**,
  **mountnet**, and
  **unmountnet** options have been added to :ref:`pc-sysconfig`.

* The "Domain Name" field has been added to :menuselection:`Control Panel --> Network Configuration --> Network Configuration (Advanced)`.

* The "Replication Server" screen has been removed from the :ref:`Life Preserver` initial configuration wizard and an option has been added to the last screen of the
  wizard offering to open the advanced configuration options so that replication can be configured.

* Life Preserver's "Local Snapshots" tab now allows you to create a list of datasets to exclude when creating snapshots.

* Life Preserver's "Replication" tab now allows you to create a list of datasets to exclude when replicating to the remote server.

.. index:: Linux
.. _PC-BSD® for Linux Users:

PC-BSD® for Linux Users
========================

PC-BSD® is based on FreeBSD, meaning that it is not a Linux distribution. If you have used Linux before, you will find that some features that you are used
to have different names on a BSD system and that some commands are different. This section covers some of these differences.

.. index:: filesystems
.. _Filesystems:

BSD and Linux use different filesystems during installation. Many Linux distros use EXT2, EXT3, EXT4, or ReiserFS, while PC-BSD® uses OpenZFS. This means
that if you wish to dual-boot with Linux or access data on an external drive that has been formatted with another filesystem, you will want to do a bit of
research first to see if the data will be accessible to both operating systems.

Table 1.3a summarizes the various filesystems commonly used by desktop systems. Most of the desktop managers available from PC-BSD® should automatically
mount the following filesystems: FAT16, FAT32, EXT2, EXT3 (without journaling), EXT4 (read-only), NTFS5, NTFS6, and XFS. See the section on
:ref:`Files and File Sharing` for more information about available file manager utilities.

**Table 1.3a: Filesystem Support on PC-BSD®**

+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| Filesystem | Native to         | Type of non-native support                     | **Usage notes**                                                          |
+============+===================+================================================+==========================================================================+
| Btrfs      | Linux             | none                                           |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| exFAT      | Windows           | none                                           | requires a license from Microsoft                                        |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| EXT2       | Linux             | r/w support loaded by default                  |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| EXT3       | Linux             | r/w support loaded by default                  | since EXT3 journaling is not supported, you will not be able to mount    |
|            |                   |                                                | a filesystem requiring a journal replay unless you :command:`fsck` it    |
|            |                   |                                                | using an external utility such as                                        |
|            |                   |                                                | `e2fsprogs <http://e2fsprogs.sourceforge.net>`_                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| EXT4       | Linux             | r/o support loaded by default                  | EXT3 journaling, extended attributes, and inodes greater than 128 bytes  |
|            |                   |                                                | are not supported; EXT3 filesystems converted to EXT4 may have better    |
|            |                   |                                                | performance                                                              |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| FAT16      | Windows           | r/w support loaded by default                  |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| FAT32      | Windows           | r/w support loaded by default                  |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| HFS+       | Mac OS X          | none                                           | older Mac versions might work with                                       |
|            |                   |                                                | `hfsexplorer <http://www.catacombae.org/hfsexplorer>`_                   |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| JFS        | Linux             | none                                           |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| NTFS5      | Windows           | full r/w support loaded by default             |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| NTFS6      | Windows           | r/w support loaded by default                  |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| ReiserFS   | Linux             | r/o support is loaded by default               |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| UFS2       | FreeBSD           | check if your Linux distro provides ufsutils;  |                                                                          |
|            |                   | r/w support on Mac; UFS Explorer can be used   |                                                                          |
|            |                   | on Windows                                     | changed to r/o support in Mac Lion                                       |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+
| ZFS        | PC-BSD, FreeBSD   |                                                |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+

.. index:: devices

Linux and BSD use different naming conventions for devices. For example: 

* in Linux, Ethernet interfaces begin with :file:`eth`; in BSD, interface names indicate the name of the driver. For example, an Ethernet interface may be
  listed as :file:`re0`, indicating that it uses the Realtek :file:`re` driver. The advantage of this convention is that you can read the **man 4** page for
  the driver (e.g. type :command:`man 4 re`) to see which models and features are provided by that driver.

- BSD disk names differ from Linux. IDE drives begin with :file:`ad` and SCSI and USB drives begin with :file:`da`.

Some of the features used by BSD have similar counterparts to Linux, but the name of the feature is different. Table 1.3b provides some common examples: 

**Table 1.3b: Names for BSD and Linux Features**

+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| PC-BSD                                         | Linux                                | **Description**                                                    |
+================================================+======================================+====================================================================+
| IPFW                                           | iptables                             | default firewall                                                   |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| :file:`/etc/rc.d/` for operating system and    | :file:`rc0.d/`, :file:`rc1.d/`, etc. | in PC-BSD the directories containing the startup scripts do not    |
| :file:`/usr/local/etc/rc.d/` for applications  |                                      | link to runlevels as there are no runlevels; system startup        |
|                                                |                                      | scripts are separated from third-party application scripts         |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| :file:`/etc/ttys` and :file:`/etc/rc.conf`     | :command:`telinit`, :file:`init.d/`  | terminals are configured in *ttys* and *rc.conf* indicates which   |
|                                                |                                      | services will start at boot time                                   |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+

If you are comfortable with the command line, you may find that some of the commands that you are used to have different names on BSD. Table 1.3c lists some
common commands and what they are used for.

**Table 1.3c: Common BSD and Linux Commands**

+-----------------------------------+------------------------------------------------------------+
| Command                           | **Used to:**                                               |
+===================================+============================================================+
| :command:`dmesg`                  | discover what hardware was detected by the kernel          |
+-----------------------------------+------------------------------------------------------------+
| :command:`sysctl dev`             | display configured devices                                 |
+-----------------------------------+------------------------------------------------------------+
| :command:`pciconf -l -cv`         | show PCI devices                                           |
+-----------------------------------+------------------------------------------------------------+
| :command:`dmesg | grep usb`       | show USB devices                                           |
+-----------------------------------+------------------------------------------------------------+
| :command:`kldstat`                | list all modules loaded in the kernel                      |
+-----------------------------------+------------------------------------------------------------+
| :command:`kldload <module>`       | load a kernel module for the current session               |
+-----------------------------------+------------------------------------------------------------+
| :command:`pbi_add -r <pbiname>`   | install software from the command line                     |
+-----------------------------------+------------------------------------------------------------+
| :command:`sysctl hw.realmem`      | display hardware memory                                    |
+-----------------------------------+------------------------------------------------------------+
| :command:`sysctl hw.model`        | display CPU model                                          |
+-----------------------------------+------------------------------------------------------------+
| :command:`sysctl hw.machine_arch` | display CPU Architecture                                   |
+-----------------------------------+------------------------------------------------------------+
| :command:`sysctl hw.ncpu`         | display number of CPUs                                     |
+-----------------------------------+------------------------------------------------------------+
| :command:`uname -vm`              | get release version information                            |
+-----------------------------------+------------------------------------------------------------+
| :command:`gpart show`             | show device partition information                          |
+-----------------------------------+------------------------------------------------------------+
| :command:`fuser`                  | list IDs of all processes that have one or more files open |
+-----------------------------------+------------------------------------------------------------+

The following articles and videos provide additional information about some of the differences between BSD and Linux: 

* `Comparing BSD and Linux <http://www.freebsd.org/doc/en/articles/explaining-bsd/comparing-bsd-and-linux.html>`_

* `FreeBSD Quickstart Guide for Linux® Users <http://www.freebsd.org/doc/en/articles/linux-users/index.html>`_

* `BSD vs Linux <http://www.over-yonder.net/~fullermd/rants/bsd4linux/01>`_

* `Why Choose FreeBSD? <http://www.freebsd.org/advocacy/whyusefreebsd.html>`_

* `Interview: BSD for Human Beings <http://www.unixmen.com/bsd-for-human-beings-interview/>`_

* `Video: BSD 4 Linux Users <https://www.youtube.com/watch?v=xk6ouxX51NI>`_

* `Why you should use a BSD style license for your Open Source Project <http://www.freebsd.org/doc/en/articles/bsdl-gpl/article.html>`_

* `A Sysadmin's Unixersal Translator (ROSETTA STONE) <http://bhami.com/rosetta.html>`_
