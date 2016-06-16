
**Preface** 

Written by users of the TrueOS® operating system.

Version |version|

Copyright © 2005 - 2016 The TrueOS® Project.

Welcome to TrueOS®! This Handbook covers the installation and use of
TrueOS® |version|. This Handbook is a work in progress and relies on th
e contributions of many individuals. If you are interested in assisting
with the Handbook, refer to the documentation
`README <https://github.com/pcbsd/pcbsd/blob/master/src-qt5/docs/README.md>`_. 
If you use IRC Freenode, you are welcome to join the #pcbsd channel
where you will find other TrueOS® users.

Previous versions of the Handbook in various formats and languages are
available from `here <http://www.pcbsd.org/docs/>`_. 

The TrueOS® Users Handbook is freely available for sharing and
redistribution under the terms of the
`Creative Commons Attribution License <https://creativecommons.org/licenses/by/4.0/>`_. 
This means that you have permission to copy, distribute, translate, and
adapt the work as long as you attribute the TrueOS® Project as the
original source of the Handbook.

TrueOS® and the TrueOS® logo are registered trademarks of
`iXsystems <https://www.ixsystems.com/>`_. If you wish to use the
TrueOS® logo in your own works, ask for permission first from
marketing@ixsystems.com.

AMD is a trademark of Advanced Micro Devices, Inc.

Apache is a trademark of The Apache Software Foundation.

AppCafe® is a registered trademark of
`iXsystems <https://www.ixsystems.com/>`_.

Asus® and Eee PC® are registered trademarks of ASUSTeK® Computer Inc.

Facebook® is a registered trademark of Facebook Inc.

Flash® is a registered trademark of Adobe Systems Incorporated in the
United States and/or other countries.

FreeBSD® is a registered trademark of the
`FreeBSD Foundation <https://www.freebsdfoundation.org/>`_. 

FreeNAS® is a registered trademark of
`iXsystems <https://www.ixsystems.com/>`_.

Intel, the Intel logo, Pentium Inside, and Pentium are trademarks of
Intel Corporation in the U.S. and/or other countries.

Java™ is a trademark of Oracle America and/or its affiliates in the
United States and other countries.

LinkedIn® is a registered trademark of LinkedIn Corporation.

Linux® is a registered trademark of Linus Torvalds.

Mac and Mac OS are trademarks of Apple Inc., registered in the U.S. and
other countries.

NVIDIA® is a trademark and/or registered trademark of NVIDIA Corporation
in the U.S. and other countries.

ThinkPad® is a registered trademark of Lenovo.

Twitter is a trademark of Twitter, Inc. in the United States and other
countries.

UNIX® is a registered trademark of The Open Group.

VirtualBox® is a registered trademark of Oracle.

VMWare® is a registered trademark of VMWare, Inc.

Windows® is a registered trademark of Microsoft Corporation in the
United States and other countries.

**Typographic Conventions** 

* Names of graphical elements such as buttons, icons, fields, columns,
  and boxes are enclosed within quotes. For example: click the "Browse
  Categories" button.

* Menu selections are italicized and separated by arrows. For example:
  :menuselection:`Control Panel --> About`.

* Commands that are mentioned within text are highlighted in
  :command:`bold text`. Command examples and command output are
  contained within code blocks.

* File names are enclosed in a blue box :file:`/like/this`.

* Keystrokes are formatted in a blue box. For example: press
  :kbd:`Enter`.

* **bold text** is used to emphasize an important point.

* *italic text* is used to represent device names or text that is input
  into a GUI field.

Introduction
************

Welcome to TrueOS®!

`TrueOS® <http://www.pcbsd.org/>`_ (formerly known as PC-BSD) began in
2005 when Kris Moore presented the first beta version of a FreeBSD
operating system pre-configured for desktop use. Since then, TrueOS® has
matured into a polished, feature-rich, free-of-charge, open source
operating system that meets the desktop or server needs of the beginner
to the advanced user alike.

TrueOS® is essentially a customized installation of FreeBSD, not a
forked derivative. Since the underlying FreeBSD system has been kept
intact, you have a fully functional FreeBSD system under the hood.
TrueOS® provides an easy-to-use installer which can be used to install a
desktop or a server version of FreeBSD. Other differences from FreeBSD
include: 

* TrueOS® pre-configures the BSD-licensed, :ref:`Lumina Desktop`
  environment during a desktop installation. Additional desktop
  environments can be installed and will appear in the login menu,
  allowing the user to select  which environment to log into.

* The TrueOS® installer supports configuring ZFS and encryption during
  installation.

* TrueOS® provides both a graphical and command line software management
  system.

* TrueOS® provides many graphical utilities for configuring and managing
  the system. These utilities have both a command line equivalent and
  a REST and WebSocket API so that they can also be used to manage
  multiple systems.

* TrueOS® comes pre-configured with a number of automatic scripts to
  perform tasks such as connecting digital cameras or USB memory sticks.

* The TrueOS® boot menu supports boot environments, or snapshots of the
  operating system, and the TrueOS® Update Manager automatically adds a
  new boot environment to the boot menu before updating the operating
  system or software. This means that if an update fails, you can reboot
  into the previous version of the operating system, before the update
  occurred.

While it began as an independent project, since October 2006 TrueOS® is
financially backed and supported by the enterprise-class hardware
solutions provider `iXsystems <https://www.ixsystems.com/>`_.

.. index:: features
.. _Goals and Features:

Goals and Features
==================

TrueOS® provides the following features: 

* **Easy installation:** to install either a graphical desktop or
  command-line server, simply insert the installation media, reboot the
  system to start the installer, and answer a few questions in the
  installation menus.

* **Automatically configured hardware:** video, sound, network, and
  other devices are configured automatically during installation.

* **Intuitive desktop interface:** TrueOS® installs the
  :ref:`Lumina Desktop` and additional desktop environments can be
  installed to support your day-to-day computing needs.

* **Easy software management:** with :ref:`AppCafe®`, installing,
  upgrading, and uninstalling software is safe and easy.

* **Lots of software available:** :ref:`AppCafe®` can be used to install
  software that has been ported to FreeBSD (currently over 26,100
  applications).

* **Easy to update:** TrueOS® provides a built-in :ref:`Update Manager`
  that provides notifications of available updates. This utility makes
  it easy to apply operating system security fixes, bug fixes, and
  system enhancements as well as upgrade to newer versions of the
  operating system or installed software.

* **Virus-free:** TrueOS® is not affected by viruses, spyware, or other
  malware.

* **No defragmentation:** TrueOS® hard drives do not need to be
  defragmented and are formatted with OpenZFS which is a self-healing
  filesystem.

* **Laptop support:** provides power saving, swap space encryption, and
  automatic switching between wired and wifi network connections.

* **Secure environment:** TrueOS® provides a pre-configured firewall and
  a built-in host-based Intrusion Detection System.

* **Easy system administration:** TrueOS® provides many graphical tools
  for performing system administration tasks.

* **Localization:** TrueOS® supports a number of native languages and
  locales.

* **Vibrant community:** TrueOS® has a friendly and helpful community. 

.. index:: What's New
.. _What's New in |version|:

What's New in |version|
=======================

The following features or enhancements were introduced for TrueOS®
|version|:

* Based on FreeBSD 11.0 which adds these
  `features <https://www.freebsd.org/releases/11.0R/relnotes.html>`_.

* The GRUB bootloader has been replaced by the FreeBSD bootloader which
  now provides both GELI and boot environment support. The "Use GRUB
  bootloader" checkbox has been added to the "Customize" Disk Selection
  screens for users of dual-boot systems who prefer to use the GRUB boot
  loader.

* A TrueOS® installation installs the :ref:`Lumina Desktop`. Additional
  window managers can be installed using :ref:`AppCafe®`.

* The :ref:`SysAdm™ Client` and server have been added and most of the
  :ref:`Control Panel` utilities have been rewritten to use the SysAdm™
  middleware. Under the hood, SysAdm™ provides REST and WebSocket APIs
  for securely managing local and remote FreeBSD and TrueOS® systems.

* :command:`freebsd-update` has been retired in favor of using
  :command:`pkg` for system updates.

* The option to use the SCFB display driver has been added to the
  installer. This driver is suitable for newer UEFI laptops as it
  automatically detects native resolution and is a good solution for
  newer Intel drivers that have not been ported yet to FreeBSD. Before
  selecting this driver, check the BIOS and make sure the CSM module is
  disabled. This driver does not support a dual-head configuration, such
  as an external port for presentations, or suspend and resume.

* The "Customize" button has been removed from the
  :ref:`System Selection Screen` in order to reduce the size of the
  installation media. Additional software can be installedg
  post-installation using :ref:`AppCafe®`.
  
* The "Boot to console (Disable X)" option has been added to the 
  graphical boot menu.

* The following utilities have been removed from :ref:`Control Panel` as
  they have been converted to the SysAdm™ API and are available in the
  :ref:`SysAdm™ client`: :ref:`AppCafe®`, :ref:`Update Manager`,
  :ref:`Boot Environment Manager`, and :ref:`Life Preserver`.
  
* These new utilites are available in the :ref:`SysAdm™ Client`:
  :ref:`Manage SSL Keys` and :ref:`Task Manager`.  

* The graphical and command line versions of PBI Manager and Warden have
  been removed.

.. index:: Linux
.. _TrueOS® for Linux Users:

TrueOS® for Linux Users
========================

TrueOS® is based on FreeBSD, meaning that it is not a Linux
distribution. If you have used Linux before, you will find that some
features that you are used to have different names on a BSD system and
that some commands are different. This section covers some of these
differences.

.. index:: filesystems
.. _Filesystems:

BSD and Linux use different filesystems during installation. Many Linux
distros use EXT2, EXT3, EXT4, or ReiserFS, while TrueOS® uses OpenZFS.
This means that if you wish to dual-boot with Linux or access data on an
external drive that has been formatted with another filesystem, you will
want to do a bit of research first to see if the data will be accessible
to both operating systems.

Table 1.3a summarizes the various filesystems commonly used by desktop
systems. TrueOS® should automatically mount the following filesystems:
FAT16, FAT32, EXT2, EXT3 (without journaling), EXT4 (read-only), NTFS5,
NTFS6, and XFS. See the section on :ref:`Files and File Sharing` for a
comparison of some graphical file manager utilities.

**Table 1.3a: Filesystem Support on TrueOS®**

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
| ZFS        | TrueOS, FreeBSD   |                                                |                                                                          |
+------------+-------------------+------------------------------------------------+--------------------------------------------------------------------------+

.. index:: devices

Linux and BSD use different naming conventions for devices. For example: 

* in Linux, Ethernet interfaces begin with :file:`eth`; in BSD,
  interface names indicate the name of the driver. For example, an
  Ethernet interface may be listed as :file:`re0`, indicating that it
  uses the Realtek :file:`re` driver. The advantage of this convention
  is that you can read the **man 4** page for the driver (e.g. type
  :command:`man 4 re`) to see which models and features are provided by
  that driver.

* BSD disk names differ from Linux. IDE drives begin with :file:`ad` and
  SCSI and USB drives begin with :file:`da`.

Some of the features used by BSD have similar counterparts to Linux, but
the name of the feature is different. Table 1.3b provides some common
examples: 

**Table 1.3b: Names for BSD and Linux Features**

+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| TrueOS                                         | Linux                                | **Description**                                                    |
+================================================+======================================+====================================================================+
| IPFW                                           | iptables                             | default firewall                                                   |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| :file:`/etc/rc.d/` for operating system and    | :file:`rc0.d/`, :file:`rc1.d/`, etc. | in TrueOS the directories containing the startup scripts do not    |
| :file:`/usr/local/etc/rc.d/` for applications  |                                      | link to runlevels as there are no runlevels; system startup        |
|                                                |                                      | scripts are separated from third-party application scripts         |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+
| :file:`/etc/ttys` and :file:`/etc/rc.conf`     | :command:`telinit`, :file:`init.d/`  | terminals are configured in *ttys* and *rc.conf* indicates which   |
|                                                |                                      | services will start at boot time                                   |
+------------------------------------------------+--------------------------------------+--------------------------------------------------------------------+

If you are comfortable with the command line, you may find that some of
the commands that you are used to have different names on BSD. Table
1.3c lists some common commands and what they are used for.

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
| :command:`pkg install <pkgname>`  | install software from the command line                     |
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

The following articles and videos provide additional information about
some of the differences between BSD and Linux: 

* `Comparing BSD and Linux <http://www.freebsd.org/doc/en/articles/explaining-bsd/comparing-bsd-and-linux.html>`_

* `FreeBSD Quickstart Guide for Linux® Users <http://www.freebsd.org/doc/en/articles/linux-users/index.html>`_

* `BSD vs Linux <http://www.over-yonder.net/~fullermd/rants/bsd4linux/01>`_

* `Why Choose FreeBSD? <http://www.freebsd.org/advocacy/whyusefreebsd.html>`_

* `Interview: BSD for Human Beings <http://www.unixmen.com/bsd-for-human-beings-interview/>`_

* `Video: BSD 4 Linux Users <https://www.youtube.com/watch?v=xk6ouxX51NI>`_

* `Why you should use a BSD style license for your Open Source Project <http://www.freebsd.org/doc/en/articles/bsdl-gpl/article.html>`_

* `A Sysadmin's Unixersal Translator (ROSETTA STONE) <http://bhami.com/rosetta.html>`_
