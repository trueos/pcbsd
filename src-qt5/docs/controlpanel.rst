.. index:: configuration
.. _Control Panel:

Control Panel
*************

PC-BSD® provides a Control Panel which contains tools for managing your system. The Control Panel is available from any desktop, meaning it is available
regardless of which desktop you log into.

.. note:: if a desktop does not contain an icon or menu item for Control Panel, type :command:`pc-controlpanel` from a shell prompt to launch the Control
   Panel.

A screenshot of Control Panel started from the KDE desktop can be seen in Figure 8.a.

**Figure 8.a: PC-BSD® Control Panel**

.. image:: images/control1.png

The available utilities are divided into sections. If you click a grey section bar, you can toggle between displaying (bar has up arrow) or hiding (bar has
down arrow) its icons. In this example, the display for the "Desktop environment" section is hidden.

The search box in the upper right can be used to find the proper control panel item if you know what you would like to configure but are uncertain which
utility to use. The icon next to the search box can be used to change the size of the icons, change the view from a grid to a list, and organize the icons
into a fixed layout.

If an icon includes a yellow exclamation mark, you will need to input your password in order to access that configuration utility.

.. note:: if your user account is not a member of the *wheel* group, you will not see the configuration utilities in Control Panel that require a password.
   By default, the first user account that you create is made a member of the *wheel* group. You can log in as that user and use :ref:`User Manager` to add
   other accounts to this group.

Control Panel includes a "desktop selector" menu which allows you to load the configuration utilities from all installed desktops, KDE, GNOME, Cinnamon, MATE,
XFCE4, LXDE, or Lumina, assuming that they are installed, or just the utilities that came with PC-BSD®. Figure 8.b shows the desktop selector menu in use. In
this example, the user is currently logged into the LXDE desktop but they have chosen to view the GNOME utilities. The menu icon indicates the control panel
view while "(current)" will be beside the desktop that is presently active.

**Figure 8.b: Desktop Selector Menu** 

.. image:: images/control2.png

Switching between the icons in the selector changes the icons displayed within the control panel window to match those used in that desktop. If "All desktops"
is set by the desktop selector, you will see every utility that is available, depending upon which desktops are currently installed. You can change which
desktops are installed using :ref:`AppCafe®`.

The following utilities are found in the Control Panel of a PC-BSD® system, regardless of the desktop that is installed: 

**Software and updates** 

* :ref:`AppCafe®`

* :ref:`EasyPBI`

* :ref:`Update Manager`

**System management** 

* :ref:`About`

* :ref:`Active Directory & LDAP`

* :ref:`Boot Manager`

* :ref:`Hardware Compatibility`

* :ref:`Login Manager`

* :ref:`Service Manager`

* :ref:`System Manager`

* :ref:`User Manager`

**Hardware** 

* :ref:`Disk Manager`

* :ref:`Display`

* :ref:`Mount Tray`

* :ref:`PC-BSD Keyboard Settings`

* :ref:`PC-BSD Mixer Tray`

* :ref:`Printing`

* :ref:`Scanner`

**Networking** 

* :ref:`Firewall Manager`

* :ref:`Network Configuration`

**Tools** 

* :ref:`Life Preserver`

* :ref:`Report a bug`

* :ref:`Warden®`

.. index:: configuration
.. _EasyPBI:

EasyPBI
=======

PBIng provides an information wrapper around existing FreeBSD packages. This wrapper, known as a PBI module, contains the metadata which displays information
about the PBI in :ref:`AppCafe®`, such as screenshots, similar applications, search terms, and plugins. With PBIng, you no longer have to build PBIs.
Instead, you can modify the information contained in PBI modules in order to create a custom PBI repository which can be added to AppCafe®. Since PBI modules
are comprised of ASCII text files, they can be easily edited using the graphical EasyPBI utility or manually with a text editor.

This chapter demonstrates how to use EasyPBI, which is the recommended method for customizing PBI modules. It then describes the files contained in a PBI
module for those users who prefer to edit files manually or who want a better understanding of the components of a PBI module. Once you have created your
custom modules, refer to :ref:`Create Your Own PBI Repository` for instructions on how to add the custom repository to AppCafe®. 

.. note:: if your goal is to make a change to a single PBI, not a custom repository, refer to :ref:`Make Minor Modifications to a PBI Module`. 

.. index:: EasyPBI
.. _Creating a PBI Module:

Creating a PBI Module 
----------------------

EasyPBI can be launched from :ref:`Control Panel` or by typing :command:`EasyPBI`.


.. note:: do not use EasyPBI while :ref:`AppCafe®` is running as both applications place a lock on the package database.

When you first launch EasyPBI, everything will be greyed out, as seen in Figure 8.1a. This is because you have not created any modules yet.

**Figure 8.1a: Initial EasyPBI Graphical Interface**

.. image:: images/easypbi1.png

Click the "New" button to create a PBI module and to open the screen shown in Figure 8.1b.

**Figure 8.1b: Create a New Module Screen**

.. image:: images/easypbi2.png

The following options are available when creating a new module:

* **FreeBSD Package:** click the "Find" button to browse the available categories and to select the package to convert into a PBI.

* **Icon File:** by default, a generic PBI icon will be used. If the application has its own icon, use the "Select" button to browse to the location of the
  icon. When selecting a custom icon, use a 64x64 .png file with a transparent background.

* **Quick Module:** check this box if the system is not currently connected to the Internet. Otherwise, EasyPBI does a scan of the package in order to
  automatically fill in the module's information. This information can be filled in manually, as described in the next screen.

After making your selections, click "OK". The information for the module will appear as seen in the example in Figure 8.1c. In this example, the
:file:`net-p2p/linuxdcpp` port has been selected.

**Figure 8.1c: PBI Configuration Screen**

.. image:: images/easypbi3.png

The "Port/Package" and "Author" fields are mandatory and should be auto-filled for you, unless you checked the "Quick Module" box. If the port does not supply
the "Author" name, check the application's website to see if you can find one. Otherwise, input the email address of the port maintainer. A generic icon will
be supplied for the module. You can change the default icon by clicking it.

The other items in the "PBI Configuration" tab are optional:

* **App Type:** if this is empty, the PBI will not appear in an AppCafe® search unless "Search all available PBI and packages" is checked in the "App Search" tab. Otherwise,
  click the green arrow to select "Graphical", "Text", or "Server". The PBI will be assigned the icon for that search selection. 

* **Search Tags:** a comma delimited with no space list of tags. If a user types one of the tags into the search bar of AppCafe®, the PBI will be listed.

* **Plugins:** if the application, such as a web browser, has associated plugins, click the "+" button to browse to the location of the plugin packages. These
  will be added to the "Plugins" tab for the PBI in AppCafe®. 

* **Screenshots:** to include a screenshot of the application, click the "+" button and browse to the location of the screenshot in :file:`.jpg` or
  :file:`.png` format. The added screenshot(s) will appear in the "Screenshots" tab for the PBI in AppCafe®. 

* **Similar Apps:** if there are any other packages with similar functionality, click the "+" button to browse to the location of the plugin packages. These
  will be added to the "Similar" tab for the PBI in AppCafe®. 

* **View Package Overrides:** check this box to display additional settings . By default, the PBI will be built using the default options provided by the
  package. Some defaults can be overridden in this section: the default PBI name, URL for the application's website, license text, summary, and description.
  You can also add additional packages to install with the PBI or delete a package that is typically installed with the application. Note that you typically
  should not need to make any of these changes.

.. note:: changes within this screen will not be saved until you click the "Save Configuration" button. Be sure to save any changes before leaving this tab.

.. index:: EasyPBI
.. _Advanced Module Configuration:

Advanced Module Configuration
-----------------------------

The "XDG Shortcuts" tab, shown in Figure 8.1d, is used to create desktop icons and menu entries so that the application can be easily started from within a desktop
environment. This is important step for graphical applications as it configures the primary method for interacting with the program.

**Figure 8.1d: XDG Shortcuts Configuration**

.. image:: images/easypbi4.png

Any entries currently configured for the module will appear in the left side of the tab. Click an existing entry to display its details on the right. You can
remove a highlighted entry by clicking the "-" (minus sign) button, or create a new entry by clicking on the white paper button under the entry list which
will clear the fields in the right frame so that you can input new values. On the right side of this tab, you can edit the currently selected entry and click
the "Save" button to overwrite the current entry with the new settings. Alternately, click "Add" to copy the existing details to a new entry.

The "Entry Details" section of this tab are as follows when the "Desktop" button is selected: 

* **Name:** this is the text that will appear for the desktop menu entry, and is usually the full name of the application.

* **Executable:** input the name of the executable to run. EasyPBI will automatically generate the PBI-specific path to the binary.

* **Icon:** when using a custom icon, click "Custom Icon Path" and input the full path to the icon file.

* **Open in Terminal:** check this box if the application needs to be opened in an X terminal. This is useful for running some text-based programs that need
  to be embedded into a console for user interaction.

* **Make Invisible:** if checked, the entry will be hidden. This is not as useful for desktop entries but can be handy with menu entries.

* **Requires Root:** if checked, the user will be prompted for their password when the application starts. This is important if the program requires special
  users or groups to be created or an installation script needs access to the local system to make modifications.

If you click "Menu", two more fields will be added to the "Entry Details" section: 

* **Category:** indicates the menu category that the entry will be placed under when listed in the desktop environment. Click the green arrow to see the
  available menu categories. The recommended category will have a small black arrow next to it.

* **MIME Patterns:** used to associate a space-separated list of file types with the application. This is useful when paired with the "Make Invisible" option.
  For example, consider an application which has two binaries representing two different aspects of the program and an additional binary that asks which of
  the two you want to use. You could create menu entries for all three binaries, but make the two specific ones invisible and associate file types with them.
  This means that when a user tries to open one of those file types, it will automatically run the particular binary that uses it, rather than prompting the
  user for input about what to do with the file.

If you make any changes in this tab, click the "Save" button to save them.

The "Scripts" tab, shown in Figure 8.1e, is used to create custom installation and removal scripts for the PBI.

**Figure 8.1e: Scripts Configuration**

.. image:: images/easypbi5.png

If you click on the drop-down menu, you will see a list of available script types, with an icon indicating whether or not a custom script exists in the
module. Selecting a script type will activate a "Create" button if the script does not exist, or will display the full script in a box for editing.

The possible script types are: 

* **post-install.sh**: script run after installation of the PBI.

* **pre-remove.sh**: script run before deletion of the PBI.

If you add or remove any scripts in this tab, click the "Save" button to save them.

The "Service Configuration" tab, shown in Figure 8.1f, allows you to setup a remote graphical configuration interface for the application. This is generally
used for services or daemons that do not have a configuration interface and lets the user perform tasks with that service such as modifying runtime
configuration options or starting, stopping, and restarting the service. Any configurations will appear in the new AppCafe® web interface
(:command:`pc-softweb`), which allows the user to manage those services from remote systems or phones.

**Figure 8.1f: Service Configuration**

.. image:: images/easypbi6.png

The "Visual Options" list is used to setup the options for controlling the service. To add an entry to this list, click "New Option" which will open the
screen shown in Figure 8.1g. 

**Figure 8.1g: Adding a Visual Option**

.. image:: images/easypbi7.png

The following fields are available when adding a visual option. Examples for values to use in these fields can be found in the
`service configuration file for irc/bitlbee <https://github.com/pcbsd/pcbsd/blob/master/pbi-modules/irc/bitlbee/service-configfile>`_. 

* **Key:** the option to set.

* **Default Value:** the default value for the option.

* **Option Type:** supported types are *ComboBox*, *NumberBox*, or *TextBox*.

* **Name:** the name that will appear.

* **Description:** the description that will appear.

* **Options List:** appears when the *ComboBox* "Option Type" is selected. Use the "+" and "-" buttons to add or remove options to appear in the list and the
  up and down arrow buttons to order the items in the list.

* **Number Limits:** appears when the *NumberBox* "Option Type" is selected. Set the "Maximum" and "Minimum" numbers for the selection, where the default of
  *0* is unlimited.

* **Text Options:** appears when the *TextBox* "Option Type" is selected. Set the "Max Length" of allowed user input, where the default of *0* is unlimited.
  If the text should be hidden, for example when the user is inputting a password, check the box "Hide Text". 

If you create a new visual option, click the "Configuration Scripts" button as these are required for the service management configuration to work properly.
Three configuration scripts are required: 

* **getconfig.sh:** script for retrieving the current value for a given "Key" from the service configuration.

* **setconfig.sh:** script for changing a configuration value for the service.

* **doneconfig.sh:** script that is run after changing configuration values. Usually used for starting or restarting the service.

Since none of the configuration scripts are created by default, you will need to click the "Create Template" button for each script to open an editable
version of the template. Each template includes a description of the script, how it is run, and lists its input variables. Edit the template as needed and
click the "Save Script" button to save the script. Repeat for each of the three required scripts.

Once you have configured a PBI module, you can create additional modules by clicking the "New" button. To edit an existing module, click the "Load" button and
select the module name.

.. index:: EasyPBI
.. _Bulk Module Creator:

Bulk Module Creator
-------------------

When creating a custom repository, it can be convenient to quickly create all of the modules for a port category, then customize the modules as needed.
To do this, click :menuselection:`File --> Bulk Module Creator` which will open the screen shown in Figure 8.1h.

**Figure 8.1h: Bulk Module Creator**

.. image:: images/easypbi8.png

Click the icon next to "Base Directory" and browse to the location to hold the modules. For example, if the custom repository is being created in
:file:`~/myrepo`, browse to that directory.

Next, click the icon next to "Category" and select the ports category to recreate in the "Base Directory". For example, if you select the "accessibility"
category, it will create a directory called :file:`~/myrepo/accessibility/` containing subdirectories which represent the PBI modules for the existing
packages in that directory.

If the selected "Base Directory" and "Category" already exist and you want to overwrite any existing PBI modules, check the box for "Overwrite existing
modules". Otherwise, the Bulk Creator will ignore any existing modules.

If you only want to create certain types of applications, check or uncheck the boxes for the application types: "graphical", "text", "server", "other".
"Other" is any package that does not install any graphical images, does not install any files into :file:`/usr/local/bin/` or :file:`/usr/local/sbin/`, and
does not install any files into :file:`/usr/local/etc/rc.d/`. This generally occurs with packages that just install libraries or plugins, and meta-packages
which do not install anything and just have a bunch of dependencies.

After making your selections, click the "Start" button. A progress bar will indicate the status, which goes by quickly, and then summarize the number of
modules built. An example is shown in Figure 8.1i. After reviewing the summary, click the "Close" button to return to the main EasyPBI screen.

**Figure 8.1i: Summary of Modules**

.. image:: images/easypbi9.png

When creating modules, Bulk Creator will skip the following:

* any existing modules, unless "Overwrite existing modules" is checked

* any package types which were unchecked

* if the package is not found in the repository

.. note:: if all modules are skipped, check the Internet connection as Bulk Creator needs Internet access to get the package information.

Repeat for each category that you want to include in the custom repository.

.. index:: EasyPBI
.. _EasyPBI Settings:

EasyPBI Settings
----------------

To edit EasyPBI's settings, click :menuselection:`Configure --> Settings` to open the screen shown in Figure 8.1j. 

**Figure 8.1j: EasyPBI Settings** 

.. image:: images/easypbi10.png

The options in this screen allow you to configure the following: 

* **Switch User Utility:** the full path to the binary which is used to switch to administrative access. By default, it is :command:`pc-su`.

* **Auto-Detect:** if this button is clicked, a pop-up message will indicate that it will return all of the EasyPBI settings back to their defaults. Click
  "Yes" to do so or "No" to cancel the operation.

* **Modules:** the full path to the directory to save modules which are created with the "New" button.

* **Resources:** the full path to the directory to store any extra resources. These are described in :ref:`PBI Module Components`.

* **Default Icon:** the full path to the default icon used by PBI modules.

The "Configure" menu contains two other options:

* **Package Module:** when this option is clicked, a pop-up message will indicate that a copy of the current module has been packaged within the module
  directory.

* **Refresh Module:** click to refresh the module's settings.

The "Help" menu contains three options:

* **About:** displays the EasyPBI version, license, and development history.

* **FreeBSD Ports:** opens `freshports.org <http://freshports.org>`_ in the default browser.

* **PBI Modules:** opens the PBI Module Builder Guide in the default browser.

.. index:: EasyPBI
.. _PBI Module Components:

PBI Module Components 
----------------------

While EasyPBI is the recommended way for creating PBI modules, it is possible to manually create the various ASCII text files used in the modules. This
section describes the various files that comprise a PBI module. A PBI module is simply a collection of files which controls the contents of the PBI and its
appearance in :ref:`AppCafe®`. 

When creating a PBI module, create a directory on your computer to hold the module's files. For example, if you are creating a PBI module for firefox, create
the following directory using this command::

 mkdir -p ~/my_pbis/www/firefox

As you create the subdirectories and files needed by the PBI module, save them to the directory for that module.

If the application requires the user to read a license agreement, save that license as a file named :file:`LICENSE` in the directory of the PBI module. This
file is optional unless the underlying port is restricted and requires the user to accept a license in order to install and use the software.

The :file:`pbi.conf` file is mandatory. It is a simple shell script that contains the information needed to build the PBI. Here is an example of the
:file:`pbi.conf` file for firefox. When creating your file, modify the text in red to meet the needs of the PBI.
::

 #!/bin/sh
 # PBING Module Config
 # -- Program Base Information --
 PBI_ORIGIN="www/firefox"
 PBI_PROGNAME="Firefox"
 PBI_PROGWEB=""
 PBI_PROGAUTHOR="Firefox Team"
 # -- Additional repo information (optional) --
 PBI_LICENSE="MPL"
 PBI_TAGS="Firefox,Browser,Web,Mozilla,www"
 PBI_PROGTYPE="Graphical"
 PBI_CATEGORY="Web"
 # -- Additional package to install along with ORIGIN
 PBI_OTHERPKGS="www/linux-c6-flashplugin11 www/nspluginwrapper"
 # -- Optional related packages to show user
 PBI_PLUGINS="www/gecko-mediaplayer www/firefox-i18n java/icedtea-web"
 # -- Space delimited list of URLs to screenshots
 PBI_SCREENSHOTS="http://www.pcbsd.org/appcafe/screenshots/www/firefox/screen1.png http://www.pcbsd.org/appcafe/screenshots/www/firefox/screen2.png"
 # -- Other PBIs which are similar to this PBI
 PBI_RELATED="www/chromium www/opera www/seamonkey"
 export PBI_ORIGIN PBI_PROGNAME PBI_PROGWEB PBI_PROGAUTHOR
 export PBI_LICENSE PBI_TAGS PBI_PROGTYPE PBI_CATEGORY
 export PBI_OTHERPKGS PBI_PLUGINS
 export PBI_SCREENSHOTS PBI_RELATED

Table 8.1a describes the most commonly used variables.

**Table 8.1a: Commonly Used pbi.conf Variables**

+------------------+---------------------------------------------------------------------------------------------------------------------+
| Variable         | Description                                                                                                         |
+==================+=====================================================================================================================+
| PBI_ORIGIN=      | mandatory; the category/portname of the FreeBSD package                                                             |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_PROGNAME=    | mandatory; name of the application                                                                                  |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_PROGWEB=     | mandatory unless does not exist; website for the application                                                        |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_PROGAUTHOR=  | mandatory; often found at the website for the application                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_LICENSE=     | the type of open source license used by the application                                                             |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_TAGS=        | a comma separated list (no spaces) of search terms associated with the application                                  |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_PROGTYPE=    | mandatory; use "Graphical" or "Text"                                                                                |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_CATEGORY=    | the category to place the application into; click "Browse Categories" within AppCafe to see the list of categories  |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_OTHERPKGS=   | a space separated list in the format *category/portname* of other applications to bundle into the PBI               |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_PLUGINS=     | a space separated list in the format *category/portname* of similar packages                                        |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_SCREENSHOTS= | a space separated list of URLs to screenshots in *.png* or *.jpg* format                                            |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| PBI_RELATED=     | a space separated list in the format *category/portname* of similar PBIs                                            |
+------------------+---------------------------------------------------------------------------------------------------------------------+
| export           | mandatory; followed by a list of all of the variables that will be included when the PBI is built                   |
+------------------+---------------------------------------------------------------------------------------------------------------------+


The :file:`resources/` directory can contain extra files you wish copied into the PBI application directory. This is often the best place for the
:file:`LICENSE` file and other files not included with a port.

The :file:`xdg-menu/` and :file:`xdg-desktop/` directories can be used to supply menu and desktop icons, respectively. The file that you place in these
directories should be in the format :file:`pbiname.desktop`. Example 8.1a shows the :file:`firefox.desktop` files for the firefox PBI.

**Example 8.1a: Firefox XDG Entries**::

 more xdg-menu/firefox.desktop
 #!/usr/bin/env xdg-open
 [Desktop Entry] 
 Value=1.0 
 Type=Application 
 Exec=firefox %U 
 Path=
 Icon=share/pixmaps/FireFox-128.png 
 StartupNotify=true 
 Categories=Network;
 Name=Firefox 

 more xdg-desktop/firefox.desktop
 #!/usr/bin/env xdg-open 
 [Desktop Entry] 
 Value=1.0 
 Type=Application 
 Exec=firefox %U 
 Path=
 Icon=share/pixmaps/FireFox-128.png 
 StartupNotify=true 
 Name=Firefox

*Exec=* should reference the PBI's executable and any required switches.

If *Icon=* is blank, the PBI will automatically use the :file:`icon.png` located in the module's directory.

For more details on the XDG menu specifications, refer to the `freedesktop specifications <http://standards.freedesktop.org/menu-spec/menu-spec-1.0.html>`_. 

The :file:`xdg-mime/` directory is used to register file associations according to the
`freedesktop MIME specs <service configuration file for irc/bitlbee>`_. This requires the creation of an XML file. The example shown in Figure 8.1b adds the
MIME information for gimp, so that it can be available as an application choice in a web browser: 

**Example 8.1b: Gimp MIME Info**::

 more xdg-mime/gimp-xdg.xml
 <?xml version="1.0"?>
 <mime-info xmlns='http://www.freedesktop.org/standards/shared-mime-info'>
 <mime-type type="application/x-gimp">
 <comment>Gimp File</comment>
 <glob weight="100" pattern="*.xcf"/>
 <glob weight="100" pattern="*.XCF"/>
 </mime-type>
 </mime-info>

.. index:: configuration
.. _About:

About
=====

The "About" icon of Control Panel can be used to quickly find information about the PC-BSD® system. To start the application, double-click its icon in
Control Panel or type :command:`about-gui`. An example is seen in Figure 8.2a. 

**Figure 8.2a: About Information** 

.. image:: images/about1.png

The displayed information includes the version of PC-BSD® and the PC-BSD® utilities, whether the system is using the PRODUCTION or EDGE package set, the
hostname of the system, the underlying version of FreeBSD, the architecture, the name of the kernel (ident), the type of CPU, and the amount of installed
memory.

If you click the "System components" button, the X.org version and revision numbers of the PC-BSD command line and graphical utilities will be displayed, as
seen in the example shown in Figure 8.2b. 

**Figure 8.2b: System Components Screen**

.. image:: images/about2.png

If you click "Back" and then the "Desktop environments" button, the currently installed desktops and their versions will be displayed, as seen in the example
in Figure 8.2c.

**Figure 8.2c: Desktop Environments Screen**

.. image:: images/about3.png

.. index:: configuration
.. _Active Directory & LDAP:

Active Directory & LDAP
=======================

The "Active Directory & LDAP" icon is used for managing connections to an Active Directory or OpenLDAP domain. If your network contains an Active Directory or
OpenLDAP server, use this icon to input the settings needed to connect to your account information stored on the network.

This utility is to manage the settings of the client, not the Active Directory or OpenLDAP server itself. This application also needs more testing from users.
If you have trouble using this utility or find a bug, please post the details using the :ref:`Report a bug` tool.

To start the application, double-click its icon in Control Panel or type :command:`pc-su pc-adsldap`. You will be prompted to input your password. Figure 8.3a
shows the configuration utility with the Active Directory tab open.

.. note:: to prevent "DNS Update for localhost.pcbsd-3881 failed: ERROR_DNS_UPDATE_FAILED" errors, set the PC-BSD® hostname to include the realm name. For
   example, if the current hostname is "pcbsd-3881" and the realm name is "maloney.local", change the hostname to "pcbsd-3881.maloney.local" in
   :menuselection:`Control Panel --> Network Configuration (Advanced) -->  Misc`.

**Figure 8.3a: Initial Active Directory & LDAP Screen**

.. image:: images/ldap1.png

If you need to connect to a network running Active Directory, check the box "Enable Active Directory". This will change the greyed-out status of the rest of
the screen, allowing you to configure the following: 

* **Domain Name (DNS/Realm-Name):** input the name of the Active Directory domain (e.g. *example.com*) or child domain (e.g. *sales.example.com*). This
  setting is mandatory.

* **NetBIOS Name:** input the hostname of the PC-BSD® system as listed in the :ref:`About` icon.

* **Workgroup Name:** input the name of the Windows workgroup. Unless the administrator has changed it, the default workgroup name is *WORKGROUP*.

* **Allow Trusted Domains:** only check this box if the network has
  `active domain/forest trusts <http://technet.microsoft.com/en-us/library/cc757352%28WS.10%29.aspx>`_. 

* **Administrator Name:** input the name of the Active Directory Administrator account.

- **Administrator Password:** input and confirm the password for the Active Directory Administrator account.

The values that you input using this GUI are saved to :file:`/usr/local/etc/pc-activedirectory.conf` and :file:`/usr/local/etc/smb4.conf`.

.. note:: once you enable AD, you can no longer configure auto login in :ref:`Login Manager` as users will now authenticate with the Active Directory server.

Figure 8.3b shows the configuration utility with the LDAP tab open.

**Figure 8.3b: Managing LDAP Client Settings** 

.. image:: images/ldap2.png

If you need to connect to a network which contains a configured LDAP server, check the box "Enable LDAP". This will change the greyed-out status of the rest
of the screen, allowing you to configure the following: 

* **Hostname:** input the hostname or IP address of the OpenLDAP server. This setting is mandatory.

* **Base DN:** input the top level of the LDAP directory tree to be used when searching for resources (e.g. *dc=test,dc=org*).

* **Allow Anon Binding:** only check this box if the LDAP server allows read and write access without requiring authentication.

* **Root bind DN:** input the name of the administrative account on the LDAP server (e.g. *cn=Manager,dc=test,dc=org*).

* **Root bind password:** input the password for the "Root bind DN".

* **Password Encryption:** select a type supported by the LDAP server, choices are: "clear" (unencrypted), "crypt", "md5", "nds", "racf", "ad", or "exop".

* **User Suffix:** this setting is optional and is usually a department or company name. The input value will be added to the name when a user account is
  added to the LDAP directory 

* **Group Suffix:** this setting is optional and is usually a department or company name. The input value will be added to the name when a group is added to
  the LDAP directory.

* **Password Suffix:** this setting is optional. The input value will be added to the password when a password is added to the LDAP directory.

* **Machine Suffix:** this setting is optional and usually represents a description such as server or accounting. The input value will be added to the name
  when a system is added to the LDAP directory.

* **Encryption Mode:** choices are "Off", "SSL", or "TLS". The selected type must be supported by the LDAP server.

* **Self Signed Certificate:** used to verify the certificate of the LDAP server if SSL connections are used. Paste the output of the command 
  :command:`openssl s_client -connect server:port -showcerts`.

* **Auxiliary Parameters:** `ldap.conf(5) <http://www.openldap.org/software/man.cgi?query=ldap.conf>`_ options, one per line, not covered by other options in
  this screen.

The values that you input into this tab are saved to :file:`/usr/local/etc/pc-ldap.conf`.

If you are new to LDAP terminology, you may find it useful to skim through the
`OpenLDAP Software 2.4 Administrator's Guide <http://www.openldap.org/doc/admin24/>`_. 

.. index:: configuration
.. _Boot Manager:

Boot Manager
============

PC-BSD® supports a feature of ZFS known as multiple boot environments (BEs). With multiple boot environments, the process of updating software becomes a
low-risk operation as you can backup your current boot environment before upgrading or making software updates to your system. If needed, you also have the
option of booting into a backup boot environment. For example: 

* if you are making software changes to a boot environment, you can take a snapshot of that environment at any stage during the modifications.

* you can save multiple boot environments on your system and perform various updates on each of them as needed. You can install, test, and update different
  software packages on each.

* you can mount a boot environment in order to :command:`chroot` into the mount point and update specific packages on the mounted environment.

* you can move a boot environment to another machine, physical or virtual, in order to check hardware support.

.. note:: for boot environments to work properly, **do not delete the default ZFS mount points during installation.** The default ZFS layout ensures that when
   you create multiple boot environments, the :file:`/usr/pbi/`, :file:`/usr/local/`, :file:`/usr/home/`, :file:`/usr/ports/`, :file:`/usr/src/` and
   :file:`/var/` directories remain untouched. This way, if you rollback to a previous boot environment, you will not lose data in your home directories, any
   installed applications, or downloaded src or ports. During installation, you can add additional mount points, just don't delete the default ones.

To create and manage boot environments using a graphical interface, go to :menuselection:`Control Panel --> Boot Manager` or type
:command:`pc-su pc-bootconfig`. You will be prompted to enter your password.

PC-BSD® automatically creates a boot environment whenever it updates the operating system or installed software. In the example shown in Figure 8.4a, there
is an entry named *default* that represents the original installation and an entry for an operating system update to patch level 14.

**Figure 8.4a: Managing Boot Environments**

.. image:: images/be1.png

To ensure that the files that the operating system needs are included when the system boots, all boot environments include :file:`/usr`, :file:`/usr/local`,
and :file:`/var`. User-specific data is **not** included in the boot environment. This means that :file:`/usr/home`, :file:`/usr/jails`, :file:`/var/log`,
:file:`/var/tmp`, and :file:`/var/audit` will not change, regardless of which boot environment is selected at system boot.

From top to bottom, the icons on the far left are used to: 

**Create:** a new boot environment. You should do this before making any changes to the system that may impact on your current boot environment. You will be
prompted for a name which can only contain letters or numbers. Once you click "OK", the system will create the environment, then add it to the list of boot
environments.

**Remove:** will delete the highlighted boot environment. You can not delete the boot environment which has a "Running" status of *Yes* as that is the current
boot environment.

**Copy:** creates a copy of an existing boot environment.

**Rename:** used to rename the highlighted boot environment. The name is what appears in the boot menu when the system boots. You cannot rename the BE you are
currently booted into and an error message will occur if you try to do so.

**Activate:** tells the system to boot into the highlighted boot environment at next system boot. The "Default" will change to *Yes*, but the "Running" will
remain the same. In other words, "Running" refers to the boot environment the system last booted into (is currently running from) whereas "Default" indicates
which boot environment the system will boot into at next system boot.

This screen also lets you set the "Maximum auto-generated boot environments". The default is *5* and the range is from *1* to *10*. PC-BSD® automatically
creates a boot environment before updating any software and the operating system as well as before applying a system update. Once the configured maximum
number of boot environments is reached, PC-BSD® will automatically prune (delete) the oldest automatically created boot environment. However, it will not
delete any boot environments you create manually.

Whenever there are multiple boot environments, a boot menu similar to the one seen in Figure 8.4b will appear for two seconds during system boot. The menu
contains the names of the boot environments and the date each was created. Press the :kbd:`spacebar` to pause the screen so that you can review the
selections, use the arrow keys to highlight the boot environment you would like to boot into, and press :kbd:`Enter` to continue booting into the selected boot
environment. If you don't pause this screen, the system will automatically boot into either the last "Running" boot environment or, if you have activated
another boot environment, the environment that was set as the "Default". 

**Figure 8.4b: Boot Menu Shows Created Boot Environments** 

.. image:: images/be2.png

To customize this menu, click the "Grub Configuration" tab to see the screen seen in Figure 8.4c. 

**Figure 8.4c: Managing GRUB Configuration** 

.. image:: images/be3.png

The fields in this screen are used to configure the:

* **Theme File:** used to customize the look of the GRUB menu. The theme file format is described in
  `this section of the GRUB Manual <http://www.gnu.org/software/grub/manual/html_node/Theme-file-format.html>`_. The
  `GRUB 2 Theme Reference <http://wiki.rosalab.ru/en/index.php/Grub2_theme_/_reference>`_ provides additional information.

* **Font File:** before a font can be used in the GRUB menu, it must first be converted to :file:`.pf2` format using the :command:`grub-mkfont(1)` command.

* **Timer:** sets the delay time for accessing the GRUB menu. By default it is 2 seconds, so if you find that the time to access the menu goes by too quickly,
  increase this timer.

* **Custom Entries:** if you have an existing GRUB configuration that you would like to add to the menu, cut and paste it into the box. Refer to the
  `GRUB Manual <http://www.gnu.org/software/grub/manual/grub.html>`_ for more information on creating a custom GRUB configuration.

If you make any changes in this tab, the two buttons below "Settings" or "Custom Entries" will be activated. Use them to save your changes or to re-load the
GRUB configuration. If you forget to do so, a pop-up message will remind you that you have unsaved changes when you exit Boot Manager. If you do not save the
changes using these buttons, the boot menu will remain the same.

.. note:: the "Emergency Services" menu can be used to "Rebuild GRUB Menu" or to "Restore GRUB Defaults". If you make any changes to
   :file:`/boot/loader.conf`, remember to use the "Rebuild GRUB Menu" so that GRUB is aware of the changes to this file.

.. index:: boot manager
.. _Managing Boot Environments from the Command Line:

Managing Boot Environments from the Command Line
------------------------------------------------

If you are running TrueOS® or prefer to use the command line, you can manage boot environments using the :command:`beadm` command as the superuser. For
example, this command creates a boot environment named *beforeupgrade*::

 beadm create beforeupgrade
 GRUB configuration updated successfully
 Created successfully

To view all boot environments, use the :command:`list` command::

 beadm list
 BE             Active Mountpoint Space Created
 default        NR     /          9.1G  2013-12-05 09:03
 beforeupgrade  -      -          2.1M  2013-12-06 10:14

The possible flags in the "Active" field are as follows: 

* **R:** active on reboot 

* **N:** active now 

* **-:** inactive 

In this example, the current boot environment is called *default*, it is active now, will be used at next reboot, and it is mounted. The newly created
*beforeupgrade* boot environment exists, but is inactive and unmounted. To activate the new boot environment::

 beadm activate beforeupgrade
 Activated successfully
 beadm list
 BE            Active Mountpoint Space Created
 default       N      /          3.1M  2013-12-05 09:03
 beforeupgrade R      -          9.1G  2013-12-06 10:14

The flags now indicate that the system is currently booted into *default*, but at next boot the system will boot into *beforeupgrade*.

The boot menu configuration can be found in the ASCII text file :file:`/usr/local/etc/default/grub`::

 more /usr/local/etc/default/grub
 GRUB_THEME=/boot/grub/themes/pcbsd/theme.txt
 GRUB_FONT=/boot/grub/pcbsdfont.pf2
 GRUB_HIDDEN_TIMEOUT_QUIET=false
 GRUB_TIMEOUT=2

.. index:: configuration
.. _Hardware Compatibility:

Hardware Compatibility
======================

The PC-BSD® installer allows you to quickly determine if your system's video card, Ethernet card, wireless device, and sound card are compatible with
PC-BSD®. 

A "Hardware Compatibility" icon in Control Panel provides a quick overview of the system's detected hardware. To start the application, double-click its icon
in Control Panel or type :command:`pc-sysinstaller -checkhardware`.

In the example shown in Figure 8.5a, this system has a detected NVIDIA video card with a configured resolution of 1600x900, one Ethernet device using the
`em(4) <http://www.freebsd.org/cgi/man.cgi?query=em&apropos=0&sektion=4>`_ driver, and one wireless device using the
`iwn(4) <http://www.freebsd.org/cgi/man.cgi?query=iwn&apropos=0&sektion=4>`_ driver. Currently no sound card is detected, meaning that the user should
configure and test their sound card using the instructions in :ref:`PC-BSD Mixer Tray`. 

Hardware that is currently incompatible may show with a green checkbox after a system upgrade or update. This indicates that the update added the driver for
the device.

**Figure 8.5a: Sample Hardware Compatibility** 

.. image:: images/hardware.png

.. index:: configuration
.. _Login Manager:

Login Manager
=============

A Login Manager utility is available in Control Panel. Figure 8.6a shows the initial screen when you click on this icon in Control Panel or type
:command:`pc-su pc-dmconf` at the command line. Note that this utility will prompt you for your password.

**Figure 8.6a: Login Manager**

.. image:: images/login2.png

For security reasons, PC-BSD® defaults to a login screen. This means that users are required to input their password before logging into the PC-BSD® system.
If you are the only user on the PC-BSD® computer, always use the same window manager, and do not consider it a security risk for the system to automatically
boot into that window manager, you can enable auto-login using the "Auto login" tab.

As seen in the example in Figure 8.6a, the "Enable auto login" box is unchecked by default. If you check the box, the "Auto login user" drop-down menu will be
activated. Select the user account to automatically login as. If desired, the "Time Delay" can be changed to control how long the login manager will wait for
the user to cancel the automated login. Do not set this setting too low if there are times that you wish to login as a different user or to select a different
desktop. When finished, click "Apply" and you will be prompted to input the selected user's password.

.. note:: this change requires a reboot. Once the system is rebooted, a login screen will no longer appear unless the user interrupts the automatic boot or
   until this setting is changed again in Login Manager.

The "Remote login" tab, shown in Figure 8.6b, is used to enable a remote user to connect to a desktop session using
`VNC <http://en.wikipedia.org/wiki/Virtual_Network_Computing>`_. Check the "Enable Remote Desktop (VNC)" box to enable this service. You will be prompted for
the name and password of the user. Reboot in order to activate the VNC service over port 5900. You will also need to open TCP port 5900 using
:ref:`Firewall Manager`. You can test the connection using the "vnc" option of KRDC (shown in Figure 9.6a) or from another VNC client.

.. warning:: use **extreme caution** when enabling this option as it makes your system available to anyone over the network. There is an additional risk when
   a user logs in over VNC as their password is sent in clear text. If you need someone to access your PC-BSD® system to assist with troubleshooting,
   consider using :ref:`Remote Desktop` instead, which allows you to send an invitation to connect. Always disable any type of remote login **immediately**
   after finishing your troubleshooting session. If you are instead using this option to login to your desktop from a remote location such as work or school,
   configure your network's firewall to only allow VNC connections from the specific IP address you will be using to make the connection.

**Figure 8.6b: Configuring Remote Login**

.. image:: images/login3.png

The "Misc" tab is shown in Figure 8.6c.

**Figure 8.6c: Miscellaneous Options**

.. image:: images/login4.png

This screen provides the following options:

**Enable "show password" button:** by default, when a user types their password at the login prompt shown in Figure 4.8a, "*" characters are displayed as the password is
typed in order to prevent another user from seeing the password as it is typed. When the  "Enable "show password" button" box is checked, and the user clicks the lock icon next to
the typed password in the login screen, the asterisks will change to reveal the password.

**Display available users:** by default, the list of available users is displayed in the login screen. To hide this list and force the user to input their username, uncheck
this box. For security reasons, the Login Manager will refuse logins from the *root* and *toor* accounts.

**Allow Stealth Sessions:** if this box is checked, a "Stealth Session" checkbox is added to the login menu, as seen in Figure 8.6d. When a user logs into a stealth session, meaning
that they check the "Stealth Session" box in the login menu, a temporary, encrypted zvol is created, mounted, and used as a temporary home directory. When the user logs out, the
zvol is destroyed, along with the contents of that temporary home directory. This allows a user to temporarily use a PC-BSD® system without leaving any data from their login session
on the PC-BSD® system. This can be useful, for example, to allow a publicly accessible system to support multiple, transient users. It also allows you to login and run
applications as if on a fresh system each time. Should the system be rebooted before you logout of the stealth session, the onetime key is lost, rendering the data useless.
A stealth session is similar to a web browser's private mode, except for your entire desktop session.

.. warning:: if you log into a stealth session, do not save any data to your home directory as it will be destroyed at logout. If your intent is to safely interact with a
   PC-BSD® system while retaining the data in your home directory, use :ref:`PersonaCrypt` instead.

**Figure 8.6d: Logging Into a Stealth Session**

.. image:: images/stealth.png

.. index:: configuration
.. _Service Manager:

Service Manager
===============

Service Manager, seen in Figure 8.7a, provides a graphical utility for managing PC-BSD® services.

**Figure 8.7a: Managing Services Using Service Manager** 

.. image:: images/service.png

Buttons make it easy to start, stop, or restart services and to set the highlighted service to be enabled or disabled whenever the system boots. To access
this utility, go to PC-BSD® :menuselection:`Control Panel --> Service Manager` or type :command:`pc-su pc-servicemanager`. You will be prompted for your
password.

By default, services will be listed alphabetically. You can reorder the services by clicking on the "Service Name", "Running", or "Enabled" headers. Service
Manager is a graphical front-end to the rc scripts located in :file:`/etc/rc.d`.

If you do not know what a service does, do not change its settings in Service Manager. If you would like to learn more about a service, try seeing if there is
a man page for it. For example, type :command:`man apm` or :command:`man bootparamd`. If a man page does not exist, try seeing what man pages are associated
with that keyword. For example::

 apropos accounting
 ac(8) - connect time accounting
 acct(2) - enable or disable process accounting
 acct(5) - execution accounting file
 accton(8) - enable/disable system accounting
 ipfw(4) - IP packet filter and traffic accounting
 pac(8) - printer/plotter accounting information
 pam_lastlog(8) - login accounting PAM module
 sa(8) - print system accounting statistics

.. index:: configuration
.. _System Manager:

System Manager
==============

This section describes the various tasks that can be performed using the graphical System Manager utility. System Manager can be accessed from
:menuselection:`Control Panel --> System Manager` or by typing pc-su :command:`pc-sysmanager`. You will be prompted to input your password.

The "General" tab, shown in Figure 8.8a, displays the following system information: 

* the version numbers for the PC-BSD® base and its command-line and graphical utilities

* the version of the underlying FreeBSD base 

* the CPU type and speed 

* the amount of physical memory 

**Figure 8.8a: General Tab of System Manager Utility**

.. image:: images/system1.png

The "Generate" button can be used to create a report that includes the following items: 

* a listing of the installed components and their versions

* the output of the :command:`dmesg` command, which shows messages from the kernel 

* the last few lines of the :file:`/var/log/messages` log file 

* the output of the :command:`pciconf -lv` command, which lists all the devices that were found when the system booted 

* your X configuration file, which shows your display settings 

* your :file:`/etc/rc.conf` file, which shows your startup settings 

* your :file:`/boot/loader.conf` file, which shows which drivers are loaded at boot time 

* the output of the command :command:`df -m`, which shows your amount of free disk space 

* a listing from the :command:`top` command, which shows the currently running processes 

When you click the "Generate" button, you will be prompted to input the name and location of the text file that will be created. Since it is a text file, you
can view its contents in any text editor. When troubleshooting your system, this file is handy to include when you :ref:`Report a bug`.

During the installation of PC-BSD® you had an opportunity to install FreeBSD source and ports. If you did not and wish to do so after installation, use the
"Tasks" tab of System Manager, shown in Figure 8.8b.

**Figure 8.8b: Tasks Tab of the System Manager Utility**

.. image:: images/system2.png

This tab provides a graphical interface for installing system source or the ports tree using :command:`git`.

If you click the "Fetch PC-BSD System Source" button, a progress screen will indicate that sources are being downloaded to :file:`/usr/src/`. Once the
download is complete, a "Finished!" message will appear and you can click the "Close" button to exit this screen.

If you click the "Fetch PC-BSD Ports Tree" button, a message will indicate that ports are being fetched and will indicate when this is complete by adding a
"Finished!" message to the lower left corner of the message. Ports will be installed to :file:`/usr/ports/`.

The "Misc" tab of System Manager is seen in Figure 8.8c.

**Figure 8.8c: Misc Tab of the System Manager Utility**

.. image:: images/system3.png

This tab contains a checkbox to "Force IBUS keyboard input". Check this box if you wish to to input Chinese, Japanese, Korean or Indic characters using a
Latin keyboard.

.. index:: configuration
.. _User Manager:

User Manager
============

The PC-BSD® User Manager utility allows you to easily add and delete users and groups, as well as change a user's or the administrative password. To access
this utility, go to :menuselection:`Control Panel --> User Manager` or type :command:`pc-su pc-usermanager`. You will need to input your password in order to
access this utility.

.. index:: users
.. _Managing User Accounts:

Managing User Accounts
----------------------

In the example shown in Figure 8.9a, the system has two user accounts. The *dru* account has the ability to become the superuser as the "Can administrate
system" checkbox is checked.

**Figure 8.9a: Viewing User Accounts in User Manager**

.. image:: images/user1.png

If you click the "Remove" button for a highlighted user, a pop-up menu will ask if you would like to also delete the user's home directory (along with all of
their files). If you click "No", the user will still be deleted but their home directory will remain. If you have only created one user account, the "Remove"
button will be greyed out as you need at least one user to be able to login to the PC-BSD® system.

.. note:: while a removed user will no longer be listed, the user account will not actually be deleted until you click the "Apply" button. A pop-up message
   will indicate that you have pending changes if you close User Manager without clicking "Apply". If you change your mind, click "No" and the user account
   will not be deleted; otherwise, click "Yes" and the user will be deleted and User Manager will close.

The password for any user can be changed by first highlighting the user name then clicking the "Change Password" button. You will not be prompted for the old
password in order to reset a user's password; this can be handy if a user has forgotten their password and can no longer log into the PC-BSD® system. If you
click the "Change Admin Password" button, you can change the root user's password.

If you click the "Advanced View" button, this screen will change to show all of the accounts on the system, not just the user accounts that you created. An
example is seen in Figure 8.9b. 

**Figure 8.9b: Viewing All Accounts and Their Details**

.. image:: images/user2.png

The accounts that you did not create are known as system accounts and are needed by the operating system or installed applications. You should **not** delete
any accounts that you did not create yourself as doing so may cause a previously working application to stop working. "Advanced View" provides additional
information associated with each account, such as the user ID number, full name (description), home directory, default shell, and primary group. System
accounts usually have a shell of *nologin* for security reasons, meaning that an attacker can not try to login to the system using that account name.

Figure 8.9c shows the add user account creation screen that opens when you click the "Add" button.

.. note:: if you click the "Add" button while in "Simple View", you will only be prompted to enter the username, full name, and password.

**Figure 8.9c: Creating a New User Account** 

.. image:: images/user3.png

This screen is used to input the following information when adding a new user or system account: 

**Full Name:** this field provides a description of the account and can contain spaces. If it is a user account, use the person's first and last name. If it
is a system account, input a description to remind you which application uses the account.

**Username:** the name the user will use when they log in to the system; it is case sensitive and can not contain any spaces. If you are creating a system
account needed by an application, use the name provided by the application's installation instructions. If the name that you choose already exists as an
account, it will be highlighted in red and the utility will prompt you to use another name.

**Home Directory:** leave this field empty for a user account as the system will automatically create a ZFS dataset for the user's home directory under
:file:`/usr/home/username`. However, if you are creating a system account it is important to override this default by typing in :file:`/var/empty` or
:file:`/nonexistent` unless the application's installation instructions specify that the account needs a specific home directory.

**Shell:** this drop-down menu contains the shells that are available to users when they are at a command prompt. You can either keep the default or select a
shell which the user prefers.

**UID:** by default, the user will be assigned the next available User ID (UID). If you need to force a specific UID, you can set it here. Note that you
cannot set a UID lower than 1001 or specify a UID that is already in use by another user account.

**Primary Group:** if you leave the default button of "New Group" selected, a group will be created with the same name as the user. This is usually what you
want unless you are creating a system account and the installation instructions specify a different group name. Note that the drop-down menu for specifying a
group name will only show existing groups, but you can quickly create a group using the "Groups" tab.

**Password:** the password is case-sensitive and needs to be confirmed.

Once you have made your selections, press the "Save" button to create the account.

.. index:: users
.. _PersonaCrypt:

PersonaCrypt
------------

Beginning with 10.1.2, PC-BSD® provides support for PersonaCrypt. A PersonaCrypt device is a removable USB media, such as a USB stick, which has been formatted with ZFS
and encrypted with GELI. This device is used to hold a specific user's home directory, meaning that they can securely transport and access their personal files on any
PC-BSD® 10.1.2 or higher system. This can be used, for example, to securely access one's home directory from a laptop, home computer, and work computer. The device is
protected by an encryption key and a password which is, and should be, separate from the user's login password.

PersonaCrypt uses GELI's ability to split the key into two parts: one being your passphrase, and the other being a key stored on disk. Without both of these parts, the
media cannot be decrypted. This means that if somebody steals the key and manages to get your password, it is still worthless without the system it was paired with.

.. warning:: USB devices can and do eventually fail. Always backup any important files stored on the PersonaCrypt device to another device or system.

Advanced Mode can be used to initialize a PersonaCrypt device for any created user, **except** for the currently logged in user. In the example shown in Figure 8.9d, a
new user, named *dlavigne*, has been created and the entry for that user has been clicked.

**Figure 8.9d: Initialize PersonaCrypt Device** 

.. image:: images/user5.png

Before a user is configured to use PersonaCrypt on a PC-BSD® system, two buttons are available in the "PersonaCrypt" section of "Advanced Mode". Note that this section is hidden
if the currently logged in user is selected. Also, if you have just created a user and do not see these options, click "Apply" then re-highlight the user.

* **Import Key:** if the user has already created a PersonaCrypt device on another PC-BSD® system, click this button to import a previously saved copy of the key associated with
  the device. Once the key is imported, the user can now login using PersonaCrypt.

* **Initialize Device:** used to prepare the USB device that will be used as the user's home directory.

To prepare a PersonaCrypt device for this user, insert a USB stick and click "Initialize Device". A pop-up menu will indicate that the current contents of the device
will be wiped and that the device must be larger than the user's current home directory.

.. warning:: since the USB stick will hold the user's home directory and files, ensure that the stick is large enough to meet the anticipated storage needs of the home directory. Since
   the stick will be reformatted during the initialization process, make sure that any current data on the stick that you need has been copied elsewhere. Also, the faster the
   stick, the better the user experience while logged in.

Press "OK" in the pop-up menu. This will prompt you to input and confirm the password to associate with the device. Another message will ask if you are ready. Click "Yes" to
initialize the device. The User Manager screen will be greyed out while the device is prepared. Once the initialization is complete, the User Manager screen will change to
display the device's key options, as seen in Figure 8.9e.

**Figure 8.9e: PersonaCrypt Key Options** 

.. image:: images/user6.png

The following options are available:

* **Export Key:** used to create a copy of the encryption key so that it can be imported for use on another PC-BSD® system.

* **Disable Key (No Data):** used to uninitialize the PersonaCrypt device on this system. Note that the device can still be used to login to other PC-BSD® systems.

* **Disable Key (Import Data):** in addition to uninitializing the PersonaCrypt device on this system, copy the contents of the user's home directory to this system.

Once a user has been initialized for PersonaCrypt on the system, their user account will no longer be displayed when :ref:`Logging In` **unless** their PersonaCrypt device is
inserted. Once the USB device is inserted, the login screen will add an extra field, as seen in Figure 8.9f.

**Figure 8.9f: Logging in as a PersonaCrypt User** 

.. image:: images/user7.png

.. note:: if the "Allow Stealth Sessions" checkbox has been checked in :menuselection:`Control Panel --> Login Manager --> Misc`, PersonaCrypt users will still be displayed in the
   login menu, even if their USB device is not inserted. This is to allow those users the option to instead login using a stealth session. See :ref:`Login Manager` for more information
   about stealth sessions.

In the field with the yellow padlock icon, input the password for the user account. In the field with the grey USB stick icon, input the password associated with the
PersonaCrypt device.

.. warning:: To prevent data corruption and freezing the system **DO NOT** remove the PersonaCrypt device while logged in! Always log out of your session before physically
   removing the device.

.. index:: users
.. _Managing Groups:

Managing Groups
---------------

If you click the "Groups" tab, you can view all of the groups on the system, as seen in Figure 8.9g. 

**Figure 8.9g: Managing Groups Using User Manager** 

.. image:: images/user4.png

This screen has 3 columns: 

**Groups:** shows all of the groups on the system.

**Available:** shows all of the system and user accounts on the system in alphabetical order.

**Members:** indicates if the highlighted group contains any user accounts.

To add an account to a group, highlight the group name in the first column. Then, highlight the account name in the "Available" column. Click the right arrow
and the selected account will appear in the "Members" column. You should only add user accounts to groups that you create yourself or when an application's
installation instructions indicate that an account needs to be added to a group.

If you click the "Add" button, a pop-up menu will prompt you for the name of the new group. Once you press "OK", the group will be added to the "Groups"
column.

If you click the "Remove" button, the highlighted group will automatically be deleted after you press the "Apply" button, so be sure to do this with care.
Again, do not remove any groups that you did not create yourself or applications that used to work may stop working.

.. index:: configuration
.. _Disk Manager:

Disk Manager
============

The PC-BSD® Disk Manager can be used to manage ZFS pools and datasets as well as the disks attached to the system. To access this utility, use
:menuselection:`Control Panel --> Disk Manager` or type :command:`pc-su pc-zmanager` from within an xterm. You will need to input your password in order to
access this utility.

As seen in the example in Figure 8.10a, the utility will open in the "ZFS Filesystems" tab and will display the system's ZFS datasets, the amount of space
available to each dataset, and the amount of space each dataset is using.

**Figure 8.10a: Viewing the System's ZFS Datasets**

.. image:: images/disk1.png

The name of the pool in this example is *tank*. If the system has multiple pools, click the green arrow to select the desired pool.

If you right-click the pool name, the following options are available: 

* **Mount:** whether or not the filesystem can be mounted depends upon the value of the "canmount" property of the dataset.

* **Create new dataset:** Figure 8.10b shows the options that are available when you create a new dataset.

* **Create a clone dataset:** creates a copy of the dataset.

* **Take a snapshot:** will prompt for the name of the snapshot. The field is pink to remind you to type the snapshot name in immediately after the pool name
  and *@* symbol. In this example, *tank@* will be displayed in the name field. An example snapshot name could be *tank@snapshot1* or *tank@201312031353* to
  denote the date and time the snapshot was created. The snapshot creation will be instantaneous and the new snapshot will be added to the list of datasets
  and will have a camera icon. Click the entry for the snapshot entry if you wish to rename it, clone it, destroy it, rollback the system to that point in
  time, or edit its properties. If you forget when you made the snapshot, pick "Edit properties" from the snapshot's right-click menu as it will show its
  "creation" property.

* **Edit properties:** allows you modify the ZFS properties for the pool, as seen in the example shown in Figure 8.10c. The available options depend upon the
  property being modified. The options which are read-only will have a red minus sign icon next to them. ZFS options are described in :command:`man zfs` and 
  you should not change any options unless you are familiar with the ramifications of doing so.

**Figure 8.10b: Creating a New ZFS Dataset** 

.. image:: images/disk2.png

**Figure 8.10c: Editing the Pool's ZFS Properties** 

.. image:: images/disk3.png

When creating a new dataset or clone, the following options are available. Again, these options are described in :command:`man zfs` and you should not change
any options unless you are familiar with the ramifications of doing so.

* **Name:** this field is pink as a reminder to type in the dataset name immediately after the trailing "/" of the displayed pool name.

* **Prevent auto mount:** if the box is checked, the dataset will not be mounted at boot time and must instead be manually mounted as needed.

* **Mountpoint:** choices are *none*, *legacy*, or *[path]*.

* **Force UTF-8 only:** if checked, you will not be able to save any filenames that are not in the UTF-8 character code set.

* **Unicode normalization:** if checked, indicate whether unicode normalization should occur when comparing filenames, and if so, which normalization
  algorithm to use. Choices are *none*, *formD*, or *formKCF*.

* **Copies:** if checked, indicates the number of copies (0 to 3) of data to store in the dataset. The copies are in addition to any redundancy and are stored
  on different disks when possible.

* **Deduplication:** enables deduplication.
  **Do not** enable this option if the system has less than the minimum recommended 5GB of RAM per TB of storage to be deduplicated.

- **Compression:** if checked and a compression algorithm is selected in the drop-down menu, data will automatically be compressed as it is written and
  uncompressed as it is read. The algorithm determines the amount and speed of compression, where typically increased compression results in decreased speed.
  The *lz4* algorithm is recommended as it provides very good compression at near real-time speed.

To view the status of the ZFS pools and the disk(s) in the pool, click the "ZFS Pools" tab. In the example, shown in Figure 8.10d, the ZFS pool named *tank*
was created from one disk. The state of "Online" indicates that the pool is healthy.

**Figure 8.10d: Viewing the Status of the ZFS Pool** 

.. image:: images/disk4.png

If you right-click the pool, which is named *tank* in this example, the following options are available: 

* **Create new pool:** use this option if additional disks are available and you would like to create another pool instead of adding them to the existing
  pool. This will open a screen that allows you to name the new pool, select which additional disks will go into it, and select how to configure the disks.

* **Rename pool:** will prompt you to input the new name for the pool.

* **Destroy pool:** **do not select this option unless you want to destroy all of the data on the disks.**

* **Add devices:** depending upon the type of disk configuration, you may be able to extend the size of the pool by adding an equal number of disks.

* **Add log devices:** used to add an SSD or disk as a secondary ZIL.

* **Add cache devices:** used to add an SSD or disk as an L2ARC.

* **Add spare devices:** at this time, FreeBSD does not support hot spares.

* **Scrub:** will start a ZFS scrub now. This option can be I/O intensive so it isn't recommended to do this while the system is in use.

* **Export pool:** this action should be performed if you will be physically moving the disks from one system to another.

* **Properties:** used to manage the default properties of the pool. Datasets inherit the default properties, unless a property is set to a different value on
  the dataset.

If you right-click a disk entry, such as *ad0s1a* in this example, the following options are available: 

* **Attach (mirror) device:** if you wish to mirror additional disk(s), this option will open a screen which allows you to specify the disk(s) to add.

* **Take offline:** if you need to replace a bad disk, select this option before physically removing the disk.

An example of the "Disks" tab is seen in Figure 8.10e.

**Figure 8.10e: Managing Disks**

.. image:: images/disk5.png

This screen shows the size of each disk as well as the partitioning scheme. If an unformatted disk or free disk space is available, right-click the device to
format it.

.. index:: configuration
.. _Display:

Display
=======

:menuselection:`Control Panel --> Display` can be used to configure the system to run the display wizard the next time the system boots. This allows you to
reconfigure your video driver and display settings.

.. note:: if you have an NVIDIA card, double-check that "pcbsd-meta-nvidia" is installed in :ref:`AppCafe®` and install it if it is not. To check for this
   driver, search for "nvidia" in the "App Search" tab of AppCafe®. 

If you click this icon in Control Panel, you will receive the message shown in Figure 8.11a. 

**Figure 8.11a: Display Wizard Will Run at Next Boot**

.. image:: images/display2.png

Select "Yes" which will prompt for your password. You should then save your work and reboot the system.

Alternately, you can use the boot menu to start the display wizard. As soon as the system starts to boot, press the left :kbd:`Shift` button to access the
GRUB boot menu. Unless you are dual booting or have configured boot environments, there will be one entry named "PC-BSD (default)" in the boot menu. Press
:kbd:`Enter` and select "Run the Display Wizard" from the menu.

Regardless of whether you started the Display Wizard from Control Panel or from the boot menu, it will finish booting the system and then prompt you to
confirm the resolution if it finds an optimal one. To configure a different resolution, click "No" to access the display wizard, shown in Figure 8.11b. 

**Figure 8.11b: Display Settings Wizard** 

.. image:: images/display3.png

This screen can be used to select the desired screen resolution, color depth, and video driver. If you select the "vesa" driver, it will always work but will
provide sub-optimal performance. Click on the drop-down menu to select the driver that most closely matches your video card name.

You can also use the drop-down menus to change the screen resolution and color depth values. If the value you desire is not listed, it may be that the
selected driver does not support that resolution or depth.

Advanced users can select their monitor's horizontal sync and vertical refresh rate in the "Advanced" tab, seen in Figure 8.11c.

**Figure 8.11c: Advanced Tab of Display Settings**

.. image:: images/display4.png

Use caution and refer to your monitor's documentation if you make any changes here. If you are not sure what you are doing, leave the default values as-is.

If your computer is connected to two monitors, check the box "Enable Dual-Head support". 

When you are finished, click the "Apply" button for your settings to be tested. If anything goes wrong during testing, you should be taken back to the
"Display Settings" screen so that you can try another setting. Once you are satisfied with the settings, click "Yes" when prompted to accept them.

.. index:: compositing
.. _Desktop Effects and Compositing:

Desktop Effects and Compositing 
--------------------------------

To prevent problems with video cards that do not support them, desktop effects (used by KDE) and display compositing (used by XFCE) are disabled by default.
You can change this default if your video card supports desktop effects.

To enable desktop effects while logged into KDE, click :menuselection:`Favorites --> System Settings --> Desktop Effects` to access the configuration screen
shown in Figure 8.11d. Check the box "Enable desktop effects at startup". You can use the "All Effects" tab to get more information about each possible
effect and to enable the effects that interest you.

**Figure 8.11d: Enabling Desktop Effects in KDE**

.. image:: images/display5.png

To enable display compositing while logged into XFCE, go to :menuselection:`Settings --> Window Manager Tweaks --> Compositor`. In the screen shown in Figure
8.11e, check the "Enable display compositing" box to enable the compositing options.

**Figure 8.11e: Enabling Compositing in XFCE** 

.. image:: images/display6.png

.. index:: troubleshooting
.. _Display Troubleshooting:

Display Troubleshooting 
-----------------------

If you are having problems with your display settings and would like to manually edit :file:`/etc/X11/xorg.conf` or run :command:`Xorg --config`, first tell
the PC-BSD® system to not automatically start X. To do so, add this temporary line to :file:`/etc/rc.conf`, then reboot the system::

 pcdm_enable="NO"

The system will reboot to a login prompt. After logging in, try the instructions in the
`FreeBSD Handbook <http://www.freebsd.org/doc//books/handbook/x-config.html>`_ to manually configure and test Xorg. Once you have a configuration that works
for you, save it to :file:`/etc/X11/xorg.conf`. Then, remove that temporary line from :file:`/etc/rc.conf` and start PCDM::

 service pcdm start

If your graphics white-out after a suspend or resume, try running this command as the superuser::

 sysctl hw.acpi.reset_video=1

If that fixes the problem, carefully add this line to :file:`/etc/sysctl.conf`::

 hw.acpi.reset_video=1

If the monitor goes blank and does not come back, try running this command as your regular user account::

 xset -dpms

If that fixes the problem, add that line to the :file:`.xprofile` file in your home directory.

.. index:: mount
.. _Mount Tray:

Mount Tray
==========

The Mount Tray graphical application is used to facilitate the mounting and unmounting of filesystems on internal disks USB storage devices, and optical
media. It is included in the system tray, meaning that in can be used within any window manager that provides a system tray. If you remove the icon from the
system tray, you can re-add it using :menuselection:`Control Panel --> Mount Tray` or by typing :command:`pc-mounttray &`.

.. note:: if you prefer to mount devices from the command line, see the section on :ref:`pc-sysconfig`. 

In the example shown in Figure 8.12a, a USB device and a music CD are currently inserted and the user has clicked "More Options" to view the available
options.

**Figure 8.12a: Mount Tray Example**

.. image:: images/mount1.png

When you first insert a USB drive, a "New Device" message should appear in the system tray. If you click Mount Tray and the filesystem on the device is
recognized, it will automatically mount and the contents of the device will be displayed in the default file manager for the desktop. Alternately, right-click
Mount Tray and click the "Mount" button to mount the device and its contents. A list of available file managers can be found in
:ref:`File Managers and File Structure` and Table 1.3a lists which filesystems are supported by Mount Tray. If the filesystem is not recognized, a
*?* will appear next to the device. When the device is mounted, its "Mount" button changes to "Eject". When you are finished using the device, press this
"Eject" button and wait for the message indicating that it is safe to remove the device before physically removing the device. Note that you will receive a
"Device Busy" message if the file manager is still open with the device's contents. If you receive this message, press "No" to close it, close the file
manager, then press "Eject" again. This will ensure that the device is cleanly unmounted.

.. note:: while Mount Tray will allow you to physically remove a USB device without unmounting it first, it is recommended to always "Eject" the drive first.

When you first insert an optical media, such as a music CD or DVD video, a message will indicate that an optical disk is available and the  application will
open so that you can play the contents of the disk.

The following options are available in the "More Options" menu: 

* **Open Media Directory:** click this if the default file manager does not automatically open. If the desktop does not provide a default file manager, Mount
  Tray will provide an "open with" dialogue so that you can select the utility to use to browse the contents of the USB device.

* **View Disk Usage:** in the example shown in Figure 8.12b, a UFS formatted USB device is mounted at :file:`/usr/home/dru/Media/STECH-1d`. The amount of disk
  space used by the system hard drive and the USB drive is shown in both GB and as a percentage of available disk space. The Mount Tray will turn yellow if
  disk space is over 70% and red if disk space is over 90%. If the internal disk drives are partitioned with any other filesystems, these will also appear in
  Mount Tray.

* **Rescan Devices:** click this option if an entry for the USB device does not automatically appear.

* **Load ISO File:** used to mount an ISO to a memory disk. It will prompt for your password then open a browse menu so that you can browse to the location of
  the :file:`.iso` file. Once the file is selected and mounted, its contents will be displayed in the default file manager. When you are finished browsing the
  contents, close the file manager and click the "Eject" button for the memory device in Mount Tray and enter your password when prompted. As the ISO is
  unmounted, the memory disk is also detached from the system.

* **Change Settings:** as seen in Figure 8.12c, this screen allows you to configure how often Mount Tray checks the disk space used by mounted devices. Leave
  the checkbox checked if you would like it to automatically check disk space when a disk is mounted.

- **Close Tray:** click this option to remove Mount Tray from the system tray.

**Figure 8.12b: View Disk Usage Using Mount Tray**

.. image:: images/mount2.png

**Figure 8.12c: Configure Disk Space Check** 

.. image:: images/mount3.png

.. index:: mount
.. _pc-sysconfig:

pc-sysconfig
------------

The previous section described PC-BSD®'s graphical mount utility. This graphical utility has a command-line backend, :command:`pc-sysconfig`, which can be
used directly from the command line on TrueOS® systems, window managers without a system tray, or by users who prefer to use the command line.

For usage information, run the command without any options::

 pc-sysconfig
 pc-sysconfig: Simple system configuration utility
 Usage: "pc-sysconfig <command 1> <command 2> ..."
 Available Information Commands:
  "list-remdev": List all removable devices attached to the system.
  "list-mounteddev": List all removable devices that are currently mounted
  "list-audiodev": List all available audio devices
  "supportedfilesystems": List all the filesystems that are currently detected/supported by pc-sysconfig
  "devinfo <device> [skiplabel]": Fetch device information (Filesystem, Label, Type)
  "devsize <device>": Fetch device space (must be mounted)
  "usingtormode": [TRUE/FALSE] Returns whether the system is routing all traffic through TOR
  "getscreenbrightness": Returns the brightness of the first controllable screen as a percentage (0-100) or "[ERROR]" otherwise
  "systemcansuspend": [TRUE/FALSE] Returns whether the system supports the S3 suspend state

 Available Action Commands:
  "mount <device> [<filesystem>] [<mountpoint>]":
   -- This will mount the removable device on the system (with user-accessible permissions if the mountpoint needs to be created)
   -- If there is no filesystem set (or "auto" is used), it will try to use the one that is auto-detected for the device
   -- If there is no mountpoint set, it will assign a new mountpoint within the "/media/" directory based on the device label

  "unmount <device or mountpoint> [force]":
   -- This will unmount the removable device from the system
   -- This may be forced by using the "force" flag as well (not recommended for all cases)
   -- If the input device is a memory disk (/dev/md*), then it will automatically remove the memory disk from the system as well

  "load-iso <absolute path to the *.iso file>":
   -- This will load the ISO file as a memory disk on the system (making it available for mounting/browsing)

  "setdefaultaudiodevice <pcm device>":
   -- This will set the given pcm device (I.E. "pcm3") as the default audio output device

  "setscreenbrightness <percent>":
   -- This will set the brightness of all the available screens to the given percentage

   -- It is also possible to adjust the current value by supplying a [+/-] before the number

   -- For example: using "+5" as the percentage will increase the brightness by 5% for each screen

   -- This returns "[ERROR]" or "[SUCCESS]" based on whether the change could be performed

 "suspendsystem": Puts the system into the suspended state (S3)

For example, to see a listed of the supported filesystems, use::

 pc-sysconfig supportedfilesystems
 FAT, NTFS, EXT, EXT4, CD9660, UFS, REISERFS, XFS, UDF

.. index:: keyboard
.. _PC-BSD Keyboard Settings:

PC-BSD Keyboard Settings
========================

Beginning with version 10.1.1, PC-BSD® includes a graphical utility for managing the keyboard's layout settings. To start the application, double-click its
icon in Control Panel or type :command:`pc-syskeyboard` at the command line. A screenshot of this utility is seen in Figure 8.13a. 

**Figure 8.13a: Configuring Keyboard Settings**

.. image:: images/keyboard1.png

.. note:: any changes made using this utility can me saved as either for just this login session or permanently. To make the changes permanent, click the "Save to
   User Profile" button once you are finished making your changes. Otherwise, click the "Apply to Session" button. If you just click the "Close" button, your changes
   will not be saved.

Click the "Keyboard model" drop-down menu to select the type of keyboard. Note that the default model of "Generic 104-key PC" does **not** enable support for special keys
such as multimedia or Windows keys. You will need to change this default to enable support for hot keys.

To add another keyboard layout, click the "+" button, which will open the screen shown in Figure 8.13b. Highlight the desired layout. This will activate the
"Layout variant" drop-down menu where you can select to either use the "Typical" variant or a specific variant. Press "OK" to add the configured layout.

**Figure 8.13b: Adding Another Layout**

.. image:: images/keyboard2.png

To edit an existing layout, highlight it then click the icon that looks like a pencil. You can then either change the "Layout variant" for that layout or
select a different layout entirely. Selecting a different layout will replace the current layout.

If you there are multiple layout entries defined in the "Keyboard layouts" screen, you can delete a layout by highlighting it and clicking the "-" icon. Note
that this icon is greyed out when there is only one entry as at least one keyboard layout must be defined.

After creating or editing a layout, you can test it by typing some text into the "Test here" field.

To configure keyboard shortcuts, click the "Options" tab. As seen in Figure 8.13c, the default view is used to set the keyboard layout switch keys.

**Figure 8.13c: Configuring Layout Switch Keys**

.. image:: images/keyboard3.png

If you wish to set the keys for terminating the graphical session, check the box "Ctrl+Alt+Backspace terminates xorg". 

To configure many more keyboard shortcuts, click the "Advanced view" button which will open the screen shown in Figure 8.13d. In this example, several
categories have been expanded to show their options. To enable an option, check its box.

**Figure 8.13d: Configuring Keyboard Shortcuts**

.. image:: images/keyboard4.png

.. index:: sound
.. _PC-BSD Mixer Tray:

PC-BSD Mixer Tray
=================

PC-BSD® includes a graphical utility for managing the sound card's mixer settings. Desktops that include a system tray should have a speaker icon in the system tray
which can be used to access this utility. If this icon does not appear in the system tray, either double-click the "PC-BSD Mixer Tray" icon in Control Panel or
type :command:`pc-mixer &` to add it. Alternately, to open this application without adding it to the system tray, type :command:`pc-mixer -notray`.

Figure 8.14a shows an example of right-clicking the icon in the system tray.

**Figure 8.14a: Mixer Icon**

.. image:: images/sound1.png

Figure 8.14b shows the mixer application which can be opened by either clicking the "Mixer" button shown in Figure 8.14a or by typing
:command:`pc-mixer -notray`.

**Figure 8.14b: Mixer Controls**

.. image:: images/sound2.png

The "Mixer Controls" screen provides sliders to modify the left and right channels that control volume, pcm (the sound driver), the speaker, the microphone,
the recording level, and the sound provided by the monitor. Each control can be muted/unmuted individually by clicking its "Mute" or"Unmute" button, depending
upon its current mute state.

Figure 8.14c shows the "System Configuration" tab.

**Figure 8.14c: System Sound Configuration**

.. image:: images/sound3.png

This tab contains the following options: 

* **Recording Device:** use the drop-down menu to select the device to use for recording sound.

* **Default Tray Device:** use the drop-down menu to set the default slider to display in the system tray.

* **Audio Output Channel:** use the drop-down menu to change the sound device and use the "Test" button to determine that sound is working. This is sometimes
  necessary when you change audio devices. For example, if you connect a USB headset, PC-BSD® will detect the new device and will automatically change the
  audio device to the USB input. However, if you insert a headset into an audio jack, the system may not detect the new input so you will have to manually
  change the default device.

The "File" menu can be used to quit this mixer screen or to close both this screen and remove the icon from the system tray.

The "Configuration" menu provides options for accessing the "PulseAudio Mixer" and "PulseAudio Settings" utilities as well as for restarting PulseAudio.
PC-BSD® provides full `PulseAudio <http://www.freedesktop.org/wiki/Software/PulseAudio/>`_ support and these utilities can be used to configure discoverable
network sound devices and mixer levels.

For command line only systems, type :command:`mixer` from the command line to see the current sound settings::

 mixer
 Mixer vol is currently set to 0:0
 Mixer pcm is currently set to 100:100
 Mixer mic is currently set to 50:50
 Mixer mix is currently set to 60:60
 Mixer rec is currently set to 75:75
 Mixer igain is currently set to 100:100
 Mixer ogain is currently set to 100:100

If any of these settings are set to *0*, set them to a higher value, by specifying the name of the mixer setting and a percentage value up to *100*::

 mixer vol 100
 Setting the mixer vol from 0:0 to 100:100.

You can make that change permanent by creating a file named :file:`.xprofile` in your home directory that contains the corrected mixer setting.

.. index:: troubleshooting
.. _Troubleshooting Sound:

Troubleshooting Sound 
----------------------

If you only get one or two mixer settings, you need to change the default mixer channel. As the superuser, try this command::

 sysctl -w hw.snd.default_unit=1

To see if that changed to the correct channel, type :command:`mixer` again. If you still only have one or two mixer settings, try setting the
:command:`sysctl` value to *2*, and if necessary, to *3*.

Once you have all of the mixer settings and none are set to *0*, your sound should work. If it still doesn't, these resources may help you to pinpoint the
problem: 

* `Sound Section of FreeBSD Handbook <http://www.freebsd.org/doc//books/handbook/sound-setup.html>`_

* `FreeBSD Sound Wiki <http://wiki.freebsd.org/Sound>`_

If you still have problems with sound, see the section on :ref:`Finding Help` to determine which help resources are available. When reporting your problem,
include your version of PC-BSD® and the name of your sound card.

.. index:: printing
.. _Printing:

Printing
========

Like many open source operating systems, PC-BSD® uses the Common Unix Printing System (`CUPS <http://cups.org/>`_) to manage printing. Control Panel provides
a graphical front-end for adding and managing printers.

While the graphical utility is easy to use, it may or may not automatically detect your printer depending upon how well your printer is supported by an open
source print driver. This section will walk you through a sample configuration for a HP Officejet 4500 printer. Your printer may "just work", allowing you to
breeze through the configuration screens. If your printer configuration does not work, read this section more closely for hints on how to locate the correct
driver for your printer.

.. index:: printing
.. _Researching Your Printer:

Researching your Printer 
-------------------------

Before configuring your printer, it is worth the time to see if a print driver exists for your particular model, and if so, which driver is recommended. If
you are planning to purchase a printer, this is definitely good information to know beforehand. You can look up the vendor and model of the printer in the
`Open Printing Database <http://www.openprinting.org/printers>`_ which will indicate if the model is supported and if there are any known caveats with the
print driver.

Figure 8.15a shows a search for our example printer. There are two models in this series and this particular hardware supports wireless.

**Figure 8.15a: Using Open Printing Database to Locate a Driver** 

.. image:: images/print1.png

Once the model is selected, click on the "Show this printer" button to see the results, as demonstrated in Figure 8.15b. 

**Figure 8.15b: Driver Recommendation from Open Printing Database**

.. image:: images/print2.png

For this model, the HPLIP driver is recommended. In PC-BSD®, the HPLIP driver is available as an optional system component called "pcbsd-meta-hplip". You can
see if the driver is installed, and install it if it is not, using :ref:`AppCafe®`.

.. index:: printing
.. _Adding a Printer:

Adding a Printer 
-----------------

Once you know that your printer is supported, make sure that the printer is plugged into your computer or, if the printer is a network printer, that both your
computer and the printer are connected to the network. Then, go to :menuselection:`Control Panel --> Printing` or type :command:`pc-su pc-cupscfg`. Input your
password to see a window similar to Figure 8.15c. 

**Figure 8.15c: Printer Configuration Utility** 

.. image:: images/print4.png

To add a new printer, click the "+Add" button. The printing utility will pause for a few seconds as as the wizard searches to see if any printers are
connected to your computer or network. When it is finished, you should see a screen similar to Figure 8.15d. 

**Figure 8.15d: Select a Print Device** 

.. image:: images/print5.png

In this example, the wizard has found this printer and highlighted the entry for the HP OfficeJet 4500. To also install the fax capability, instead select the
driver which includes "HP Fax". The wizard should find any supported printer that is attached to the computer or network and list it as the highlighted entry
in the "Devices" frame. Click "Forward" and the wizard will attempt to load the correct driver for the device. If it is successful, it will display the screen
shown in Figure 8.15e. If it does not automatically find your printer, read the section on :ref:`Printer Troubleshooting`.

**Figure 8.15e: Describe Printer Screen**

.. image:: images/print6.png

Since the configuration wizard found this printer, the "Describe Printer" screen automatically fills out the printer model series, a description, and the
hostname of your computer, if the printer is locally attached, or the hostname of the network printer. If you wish, you can change the printer's name or
description. Once you click the "Apply" button, the wizard will ask if you would like to print a test page. Ensure the printer has paper and click "Yes" to
print the test page. If you can not print a successful test page, see the :ref:`Printer Troubleshooting` section.

Once the printer is created, a screen will open where you can set the properties of the printer. Our sample printer's properties screen is shown in Figure
8.15f.

**Figure 8.15f: Viewing the Settings of the Newly Created Printer**

.. image:: images/print7.png

You may wish to take a few minutes to review the settings in "Policies", "Access Control", "Printer Options", and "Job Options" tabs as these allow you to
configure options such as print banners, permissions, the default paper size, and double-sided printing. The available settings will vary, depending upon the
capabilities of the print driver.

.. index:: printing
.. _Manually Adding a Driver:

Manually Adding a Driver 
-------------------------

If the print configuration wizard fails, double-check that the printer is supported as described in :ref:`Researching your Printer` and that HPLIP is
installed if it is a HP printer. Also check that the printer is plugged in and powered on.

If the wizard is unable to even detect the device, try to manually add the print device. In the "Select Device" screen (Figure 8.15d) you will need to
highlight and configure the type of connection to the printer: 

**USB:** this entry will only appear if a printer is plugged into a USB port and the number of entries will vary depending upon the number of USB ports on the
system. If there are multiple USB entries, highlight the one that represents the USB port your printer is plugged into.

**Enter URI:** this option allows you to manually type in the URI to the printer. A list of possible URIs is available on the
`cups site <http://www.cups.org/documentation.php/network.html>`_. 

**AppSocket/HP JetDirect:** select this option if you are connecting to an HP network printer. You will need to input the IP address of the printer in the
"Host" field. Only change the port number if the printer is using a port other than the default of 9100. 

**IPP:** select this option if you are connecting to a printer cabled to another computer (typically running a Microsoft operating system) that is sharing the
printer using IPP. You will need to input the IP address of the printer in the "Host" field and the name of the print queue. You can then click the "Verify"
button to ensure that you can connect to the print queue.

**LPD/LPR:** select this option if you are connecting to a printer which is cabled to a Unix computer that is using LPD to share the printer. You will need to
select the hostname and queue name from the drop-down menus.

Once you have input the information for the type of printer, press "Forward" for the wizard to continue.

If the wizard is able to find the printer but is unable to locate the correct driver for the printer, it will display the screen shown in in Figure 8.15g
instead of the the "Describe Printer" screen.

**Figure 8.15g: Manually Select the Manufacturer**

.. image:: images/print8.png

Select the manufacturer name and then click "Forward" to select the model, as seen in the example in Figure 8.15h. 

**Figure 8.15h: Manually Select the Driver**

.. image:: images/print9.png

Click "Forward" and the wizard should continue to the "Describe Printer" screen.

If the selected driver does not work, go back to the "Choose Driver" screen shown in Figure 8.15g. This screen provides two additional options for installing
the driver: 

1. **Provide PPD file:** a PostScript Printer Description (PPD) is a driver created by the manufacturer that ends in a :file:`.ppd` extension. Sometimes the
   file will end with a :file:`.ppd.gz` extension, indicating that it has been compressed with :command:`gzip`. If the driver you need was not automatically
   found, see if there is a PPD file on the driver CD that came with the printer or if one is available for download from the manufacturer's website. If you
   find a PPD, select this option and browse to the location of that file. Then, click "Forward" to continue with the printer configuration.

2. **Search for a printer driver to download:** if you know the name of the driver that you are looking for, try typing its name or number into the "Search"
   box. If found, it will display in the "Printer" model drop-down menu.

.. index:: printing
.. _Printer Troubleshooting:

Printer Troubleshooting
-----------------------

Here are some solutions to common printing problems: 

- **A test page prints but it is all garbled:** this typically means that you are using the wrong driver. If your specific model was not listed, click the
  "Change" button in the "Driver Details" section of the "Settings" tab of the printer and try choosing another driver model that is close to your model
  number. If trial and error does not fix the problem, see if there are any suggestions for your model in the
  `Open Printing database <http://www.openprinting.org/printers>`_. A web search for the word "freebsd" followed by the printer model name may also help you
  to find the correct driver to use.

- **Nothing happens when you try to print:** in this case, type :command:`tail -f /var/log/cups/error_log` in a console and then print a test page. The error
  messages should appear in the console. If the solution is not obvious from the error messages, try a web search for the error message. If you are still
  stuck, post the error, the model of your printer, and your version of PC-BSD® using the :ref:`Report a Bug` tool.

.. index:: scanner
.. _Scanner:

Scanner
=======

Control Panel provides an icon for accessing `XSane <http://www.xsane.org/>`_, a graphical utility for managing scanners.

To use your scanner, make sure the device is plugged into the PC-BSD® system and click the "Scanner" icon in Control Panel or type :command:`xsane` from the
command line. A pop-up message will indicate that XSane is detecting devices and will prompt you to accept the XSane license if a device is detected.
If a device is not detected, search for your device at the `list of supported scanners <http://www.sane-project.org/sane-backends.html>`_. 

.. note:: if the scanner is part of an HP All-in-One device, make sure that the "pcbsd-meta-hplip" package is installed. You can see if the driver is
   installed, and install it if it is not, using :ref:`AppCafe®`.

Figure 8.16a shows the XSane interface running on a PC-BSD® system attached to an HP OfficeJet.

**Figure 8.16a: XSane Interface** 

.. image:: images/sane.png

The `XSane documentation <http://www.xsane.org/doc/sane-xsane-doc.html>`_ contains details on how to perform common tasks such as saving an image to a file,
photocopying an image, and creating a fax. It also describes all of the icons in the interface and how to use them.

By default, XSane uses the default browser when you click :kbd:`F1` to access its built-in documentation. How to configure the default browser varies by
window manager so you may need to do an Internet search if you need to set that configuration setting and can not find it.

.. index:: firewall
.. _Firewall Manager:

Firewall Manager
================

PC-BSD® uses the `IPFW firewall <http://www.freebsd.org/cgi/man.cgi?query=ipfw>`_ to protect your system. By default, the firewall is configured to allow all
outgoing connections, but to deny all incoming connection requests. The default rulebase is located in :file:`/etc/ipfw.rules`. Use the Firewall Manager GUI
utility to view and modify the existing firewall rules.

.. note:: typically it is not necessary to change the firewall rules. You should only add rules if you understand the security implications of doing so,
   as any custom rules will be used to allow connections to your computer.

To access the Firewall Manager, go to :menuselection:`Control Panel --> Firewall Manager` or type :command:`pc-su pc-fwmanager`. You will be prompted to input
your password. Figure 8.17a shows the initial screen when you launch this utility.

**Figure 8.17a: Firewall Manager Utility** 

.. image:: images/firewall1.png

The "General Settings" tab of this utility allows you to: 

* Determine whether or not the firewall starts when the system boots. Unless you have a reason to do so and understand the security implications, the
  "Enable Firewall on startup" box should be checked so that your system is protected by the firewall.

* "Start", "Stop", or "Restart" the firewall.

* The "Restore Default Configuration" button allows you to return to the original, working configuration.

To add or delete custom firewall rules, click the "Open Ports" tab to open the screen shown in Figure 8.17b. Note that your custom rules will allow
**incoming** connections on the specified protocol and port number.

**Figure 8.17b: Adding a New Firewall Rule** 

.. image:: images/firewall2.png

Any rules that you create will appear in this screen. To add a rule, input the port number to open. By default, "tcp" is selected. If the rule is for the
UDP protocol, click the "tcp" drop-down menu and select "udp". Once you have the protocol and port number selected, click the "Open Port" button to add the
new rule to your custom list.

If you have created any custom rules and wish to delete one, highlight the rule to delete and click the "Close Selected Ports" button to remove it from
the custom rules list.

.. note:: whenever you add or delete a custom rule, the rule will not be used until you click the "Restart" button shown in Figure 8.17a. Also, your custom
   rules are not used whenever the system is in :ref:`Tor Mode`.

Whenever you create a custom rule, test that your new rule works as expected. For example, if you create a rule to allow an SSH connection, try connecting
to your PC-BSD® system using :command:`ssh` to verify that the firewall is now allowing the connection.

.. index:: network
.. _Network Configuration:

Network Configuration
=====================

During installation, PC-BSD® configures your Ethernet interfaces to use DHCP and provides a screen to :ref:`Connect to a Wireless Network`. In most cases,
this means that your connected interfaces should "just work" whenever you use your PC-BSD® system.

For desktops that provide a system tray, a wireless configuration icon will appear if PC-BSD® detects a supported wireless card. If you hover over the
wireless icon, shown in Figure 8.18a, it will indicate if the interface is associated and provide information regarding the IP address, IPv6 address, SSID,
connection strength, connection speed, MAC address, and type of wireless device.

**Figure 8.18a: Wireless Information in System Tray** 

.. image:: images/network1.png

If you right-click the wireless icon, you will see a list of detected wireless networks. Simply click the name of a network to associate with it. The
right-click menu also provides options to configure the wireless device, start the Network Manager, restart the network (useful if you need to renew your DHCP
address), and to close the Network Monitor so that the icon no longer shows in the system tray. If you have multiple wireless devices, each will have its own
icon in the system tray. If you do not use one of the devices, click "Close the Network Monitor" to remove it from the tray.

To view or manually configure all of your network interfaces click :menuselection:`Control Panel --> Network Configuration` or type
:command:`pc-su pc-netmanager`. If a new device has been inserted (e.g. a USB wireless interface), a pop-up message will open when you start Network
Configuration, indicate the name of the new device, and ask if you would like to enable it. Click "Yes" and the new device will be displayed with the list of
network interfaces that PC-BSD® recognizes. In the example seen in Figure 8.18b, the system has one Intel Ethernet interface that uses the *em* driver and an
Intel wireless interface that uses the *wlan* driver.

**Figure 8.18b: Network Configuration Utility** 

.. image:: images/network2.png

The rest of this section describes each tab of the Network Configuration utility and demonstrate how to view and configure the network settings for both
Ethernet and wireless devices. It will then present some common troubleshooting scenarios, known issues, and suggestions for when a device does not have a
built-in driver.

.. index:: network
.. _Ethernet Adapters:

Ethernet Adapters
-----------------

If you highlight an Ethernet interface in the "Devices" tab and either click the "Configure" button or double-click the interface name, you will see the
screen shown in Figure 8.18c.

**Figure 8.18c: Network Settings for an Ethernet Interface** 

.. image:: images/network3.png

There are two ways to configure an Ethernet interface: 

1. **Use DHCP:** this method assumes that your Internet provider or network assigns your addressing information automatically using the DHCP protocol. Most
   networks are already setup to do this. This method is recommended as it should "just work". 

2. **Manually type in the IP addressing information:** this method requires you to understand the basics of TCP/IP addressing or to know which IP address you
   should be using on your network. If you do not know which IP address or subnet mask to use, you will have to ask your Internet provider or network
   administrator.

By default, PC-BSD® will attempt to obtain an address from a DHCP server. If you wish to manually type in your IP address, check the box "Assign static IP
address". Type in the IP address, using the right arrow key or the mouse to move between octets. Then, double-check that the subnet mask ("Netmask") is the
correct value and change it if it is not.

If the Ethernet network uses 802.1x authentication, check the box "Enable WPA authentication" which will enable the "Configure WPA" button. Click this button
to select the network and to input the authentication values required by the network.

By default, the "Disable this network device" box is unchecked. If you check this checkbox, PC-BSD® will immediately stop the interface from using the
network. The interface will remain inactive until this checkbox is unchecked.

The "Advanced" tab, seen in Figure 8.18d, allows advanced users to change their `MAC address <http://en.wikipedia.org/wiki/MAC_address>`_ and to use DHCP to
automatically obtain an `IPv6 address <http://en.wikipedia.org/wiki/IPv6_address>`_. Both boxes should remain checked unless you are an advanced user who has
a reason to change the default MAC or IPv6 address and you understand how to input an appropriate replacement address.

**Figure 8.18d: Advanced Tab of an Ethernet Interface's Network Settings** 

.. image:: images/network4.png

The "Info" tab, seen in Figure 8.18e, will display the current network address settings and some traffic statistics.

**Figure 8.18e: Info Tab of an Ethernet Interface's Network Settings** 

.. image:: images/network5.png

If you make any changes within any of the tabs, click the "Apply" button to activate them. Click the "OK" button when you are finished to go back to the main
Network Configuration window.

You can repeat this procedure for each network interface that you wish to view or configure.

.. index:: network
.. _Wireless Adapters:

Wireless Adapters
-----------------

If your wireless interface does not automatically associate with a wireless network, you probably need to configure a wireless profile that contains the
security settings required by the wireless network. Double-click the wireless icon in the system tray or highlight the wireless interface displayed in the
"Devices" tab of Network Configuration and click the "Configure" button. Figure 8.18f demonstrates that this system's wireless interface is currently
associated with the wireless network listed in the "Configured Network Profiles" section.

**Figure 8.18f: Wireless Configuration Window of Network Configuration Utility** 

.. image:: images/network6.png

To associate with a wireless network, click the "Scan" button to receive the list of possible wireless networks to connect to. Highlight the network you wish
to associate with and click the "Add Selected" button. If the network requires authentication, a pop-up window will prompt you for the authentication details.
Input the values required by the network then click the "Close" button. PC-BSD® will add an entry for the network in the "Configured Network Profiles"
section.

If the network is hidden, click the "Add Hidden" button, input the name of the network in the pop-up window, and click "OK".

If you add multiple networks, use the arrow keys to place them in the desired connection order. PC-BSD® will try to connect to the first profile in the list
and will move down the list in order if it is unable to connect. When finished, click the "Apply" button. A pop-up message will indicate that PC-BSD® is
restarting the network. If all went well, there should be an IP address and status of "associated" when you hover over the wireless icon in the system tray.
If this is not the case, double-check for typos in your configuration values and read the section on :ref:`Troubleshooting Network Settings`. 

PC-BSD® supports the types of authentication shown in Figure 8.18g. You can access this screen (and change your authentication settings) by highlighting an
entry in the "Configured Network Profiles" section and clicking the "Edit" button.

**Figure 8.18g: Configuring Wireless Authentication Settings** 

.. image:: images/network7.png

This screen allows you to configure the following types of wireless security: 

* **Disabled:** if the network is open, no additional configuration is required.

* **WEP:** this type of network can be configured to use either a hex or a plaintext key. If you click "WEP" then the "Configure" button, you will see the
  screen shown in Figure 8.18h. Type the key into both network key boxes. If the key is complex, check the "Show Key" box to make sure that the passwords are
  correct and that they match. Uncheck this box when you are finished to replace the characters in the key with the * symbol. A wireless access point that
  uses WEP can store up to 4 keys and the number in the key index indicates which key you wish to use.

* **WPA Personal:** this type of network uses a plaintext key. If you click "WPA Personal" then the "Configure" button, you will see the screen shown in
  Figure 8.18i. Type in the key twice to verify it. If the key is complex, you can check the "Show Key" box to make sure the passwords match.

- **WPA Enterprise:** if you click "WPA Enterprise" then the "Configure" button, you will see the screen shown in Figure 8.18j. Select the authentication
  method ("EAP-TLS", "EAP-TTLS", or "EAP-PEAP"), input the EAP identity, browse for the CA certificate, client certificate and private key file, and input and
  verify the password.

.. note:: if you are unsure which type of encryption is being used, ask the person who setup the wireless router. They should also be able to give you the
   value of any of the settings seen in these configuration screens.

**Figure 8.18h: WEP Security Settings** 

.. image:: images/network8.jpg

**Figure 8.18i: WPA Personal Security Settings** 

.. image:: images/network9.jpg

**Figure 8.18j: WPA Enterprise Security Settings** 

.. image:: images/network10.png

If you wish to disable this wireless interface, check the box "Disable this wireless device". This setting can be desirable if you want to temporarily prevent
the wireless interface from connecting to untrusted wireless networks.

The "Advanced" tab, seen in Figure 8.18k, allows you to configure the following: 

* a custom MAC address. This setting is for advanced users and requires the "Use hardware default MAC address" box to be unchecked.

* how the interface receives its IP address information. If the network contains a DHCP server, check the box "Obtain IP automatically (DHCP)". Otherwise,
  input the IP address and subnet mask to use on the network.

* the country code. This setting is not required if you are in North America. For other countries, check the "Set Country Code" box and select your country
  from the drop-down menu.

**Figure 8.18k: Advanced Tab of a Wireless Interface** 

.. image:: images/network11.png

The "Info" tab, seen in Figure 8.18l, shows the current network status and statistics for the wireless interface.

**Figure 8.18l: Info Tab of a Wireless Interface** 

.. image:: images/network12.png

.. index:: network
.. _Network Configuration (Advanced):

Network Configuration (Advanced)
--------------------------------

The "Network Configuration (Advanced)" tab of the Network Configuration utility is seen in Figure 8.18m. The displayed information is for the currently
highlighted interface. If you wish to edit these settings, make sure that the interface that you wish to configure is highlighted in the "Devices" tab.


**Figure 8.18m: Network Configuration (Advanced) tab of the Network Configuration Utility** 

.. image:: images/network13.png

If the interface receives its IP address information from a DHCP server, this screen allows you to view the received DNS information. If you wish to override
the default DNS settings or set them manually, check the "Enable Custom DNS" box. You can then set the following: 

**DNS 1:** the IP address of the primary DNS server. If you do not know which IP address to use, click the "Public servers" button to select a public DNS
server.

**DNS 2:** the IP address of the secondary DNS server.

**Search Domain:** the name of the domain served by the DNS server.

If you wish to change or set the default gateway, check the "Enable Custom Gateway" box and input the IP address of the default gateway.

The following settings can be modified in the IPv6 section: 

**Enable IPv6 support:** if this box is checked, the specified interface can participate in IPv6 networks.

**IPv6 gateway:** the IPv6 address of the default gateway used on the IPv6 network.

**IPv6 DNS 1:** the IPv6 address of the primary DNS server used on the IPv6 network. If you do not know which IP address to use, click the "Public servers"
button to select a public DNS server.

**IPv6 DNS 2:** the IPv6 address of the secondary DNS server used on the IPv6 network.

The "Misc" section allows you to configure these options: 

**System Hostname:** the name of your computer. It must be unique on your network.

**Enable wireless/wired failover via lagg0 interface:** the  interface allows you to seamlessly switch between using an Ethernet interface and a wireless
interface. If you want this functionality, check this box.

.. note:: some users experience problems using lagg. If you have problems connecting to a network using an interface that previously worked, uncheck this box
   and remove any references to "lagg" in your :file:`/etc/rc.conf` file.

If you make any changes within this window, click the "Save" button to apply them.

.. index:: network
.. _Proxy Settings:

Proxy Settings 
---------------

The "Proxy" tab, shown in Figure 8.18n, is used when your network requires you to go through a proxy server in order to access the Internet.

**Figure 8.18n: Proxy Settings Configuration** 

.. image:: images/network14.png

Check the "Proxy Configuration" check box to activate the settings. The follow settings can be configured in this screen: 

**Server Address:** enter the IP address or hostname of the proxy server.

**Port Number:** enter the port number used to connect to the proxy server.

**Proxy Type:** choices are "Basic" (sends the username and password unencrypted to the server) and "Digest" (never transfers the actual password across the
network, but instead uses it to encrypt a value sent from the server). Do not select "Digest" unless you know that the proxy server supports it.

**Specify a Username/Password:** check this box and input the username and password if they are required to connect to the proxy server.

Proxy settings are saved to the :file:`/etc/profile` and :file:`/etc/csh.cshrc` files so that they are available to the PC-BSD® utilities as well as any
application that uses :command:`fetch`.

Applications that did not come with the operating system, such as web browsers, may require you to configure proxy support using that application's
configuration utility.

If you save any changes to this tab, a pop-up message will warn that you may have to logout and back in in order for the proxy settings to take effect.

.. index:: network
.. _Configuring a Wireless Access Point:

Configuring a Wireless Access Point
-----------------------------------

Beginning with PC-BSD® 10.1, if you click the entry for a wireless device, as seen in Figure 8.18o, the right-click menu has an option to "Setup Access
Point". 

**Figure 8.18o: Setup Access Point Option**

.. image:: images/network15.png

Figure 8.18p shows the configuration screen if you select "Setup Access Point". 

**Figure 8.18p: Access Point Basic Setup**

.. image:: images/network16.png

This screen contains two options: 

- **Visible Name:** this is the name that will appear when users scan for available access points.

- **Set Password:** setting a WPA password is optional, though recommended if you only want authorized devices to use the access point. If used, the password
  must be a minimum of 8 characters.

Figure 8.18q shows the "Advanced Configuration (optional)" screen.

**Figure 8.18q: Access Point Advanced Setup**

.. image:: images/network17.png

The settings in this screen are optional and allow you to fine-tune the access point's configuration: 

- **Base IP:** the IP address of the access point.

- **Netmask:** the associated subnet mask for the access point.

- **Mode:** available modes are *11g* (for 802.11g), *11ng* (for 802.11n on the 2.4-GHz band), or *11n* (for 802.11n).

- **Channel:** select the channel to use.

- **Country Code:** the two letter country code of operation.

.. index:: network
.. _Troubleshooting Network Settings:

Troubleshooting Network Settings 
---------------------------------

While Ethernet networking usually "just works" on a PC-BSD® system, users sometimes encounter problems, especially when connecting to wireless networks.
Sometimes the problem is due to a configuration error; sometimes a driver is buggy or is not yet available. This section is meant to help you pinpoint the
problem so that you can either fix it yourself or give the developers the information they need to fix or create the driver.

.. index:: network
.. _Useful Files and Commands:

Useful Files and Commands 
^^^^^^^^^^^^^^^^^^^^^^^^^^

When troubleshooting your network configuration, use the following files and commands.

The :file:`/etc/rc.conf` file is read when the system boots up. In order for the system to configure an interface at boot time, an entry must exist for it in
this file. Entries are automatically created for you during installation for each interface that is active. An entry will be added (if it does not exist) or
modified (if it already exists) when you configure an interface using the Network Configuration utility.

Here is an example of the :file:`rc.conf` entries for an ethernet driver (*em0*) and a wireless driver (*run0*)::

 ifconfig_em0="DHCP"
 wlans_run0="wlan0"
 ifconfig_wlan0="WPA SYNCDHCP"

When reading through your own file, look for lines that begin with *ifconfig*. For a wireless interface, also look for lines containing *wlans*.

.. note:: unlike Linux interface driver names, FreeBSD/PC-BSD® interface driver names indicate the type of chipset. Each driver name has an associated man
   page where you can learn which devices use that chipset and if there are any configuration options or limitations for the driver. When reading the man
   page, do not include the interface number. In the above example, you could read :command:`man em` and :command:`man run`.


The :file:`/etc/wpa_supplicant.conf` file is used by wireless interfaces and contains the information needed to connect to a WPA network. If this file does
not already exist, it is created for you when you enter the "Configuration" screen of a wireless interface.

The :command:`ifconfig` command shows the current state of your interfaces. When reading through its output, check that your interface is listed, has a status
of "active", and has an IP address. Here is a sample :command:`ifconfig` output showing the entries for the *re0* Ethernet interface and the *run0* wireless
interface::

 re0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 1500 options=389b<RXCSUM,TXCSUM,VLAN_MTU,VLAN_HWTAGGING,VLAN_HWCSUM,WOL_UCAST,WOL_MCAST,WOL_MAGIC>
 ether 60:eb:69:0b:dd:4d
 inet 192.168.1.3 netmask 0xffffff00 broadcast 192.168.1.255
 media: Ethernet autoselect (100baseTX <full-duplex>)
 status: active

 run0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 2290
 ether 00:25:9c:9f:a2:30
 media: IEEE 802.11 Wireless Ethernet autoselect mode 11g
 status: associated

 wlan0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> metric 0 mtu 1500
 ether 00:25:9c:9f:a2:30
 media: IEEE 802.11 Wireless Ethernet autoselect (autoselect)
 status: no carrier
 ssid "" channel 10 (2457 MHz 11g)
 country US authmode WPA1+WPA2/802.11i privacy ON deftxkey UNDEF
 txpower 0 bmiss 7 scanvalid 60 protmode CTS wme roaming MANUAL bintval 0

In this example, the ethernet interface (*re0*) is active and has an IP address. However, the wireless interface (*run0*, which is associated with *wlan0*)
has a status of "no carrier" and does not have an IP address. In other words, it has not yet successfully connected to the wireless network.

The :command:`dmesg` command lists the hardware that was probed during boot time and will indicate if the associated driver was loaded. If you wish to search
the output of this command for specific information, pipe it to :command:`grep` as seen in the following examples::

 dmesg | grep Ethernet
 re0: <RealTek 8168/8111 B/C/CP/D/DP/E PCIe Gigabit Ethernet> port 0xc000-0xc0ff mem 0xd0204000-0xd0204fff,0xd0200000-0xd0203fff irq 17 at device 0.0 on pci8
 re0: Ethernet address: 60:eb:69:0b:dd:4d

 dmesg |grep re0
 re0: <RealTek 8168/8111 B/C/CP/D/DP/E PCIe Gigabit Ethernet> port 0xc000-0xc0ff mem 0xd0204000-0xd0204fff,0xd0200000-0xd0203fff irq 17 at device 0.0 on pci8
 re0: Using 1 MSI messages
 re0: Chip rev. 0x28000000
 re0: MAC rev. 0x00000000 miibus0: <MII bus> on re0
 re0: Ethernet address: 60:eb:69:0b:dd:4d
 re0: [FILTER]
 re0: link state changed to DOWN
 re0: link state changed to UP

 dmesg | grep run0
 run0: <1.0> on usbus3
 run0: MAC/BBP RT3070 (rev 0x0201), RF RT2020 (MIMO 1T1R), address 00:25:9c:9f:a2:30
 run0: firmware RT2870 loaded

If your interface does not show up in :command:`ifconfig` or :command:`dmesg`, it is possible that a driver for this card is not provided with the operating
system. If the interface is built into the motherboard of the computer, you can use the :command:`pciconf` command to find out the type of card::

 pciconf -lv | grep Ethernet
 device = 'Gigabit Ethernet NIC(NDIS 6.0) (RTL8168/8111/8111c)'

 pciconf -lv | grep wireless
 device = 'Realtek RTL8191SE wireless LAN 802.11N PCI-E NIC (RTL8191SE?)'

In this example, there is a built-in Ethernet device that uses a driver that supports the RTL8168/8111/8111c chipsets. As we saw earlier, that driver is
*re0*. The built-in wireless device was also found but the *?* indicates that a driver for the RTL8191SE chipset was not found. A web search for "FreeBSD
RTL8191SE" will give an indication of whether a driver exists (perhaps in a version of FreeBSD that has not been released yet) or if a driver is being
developed.

The FreeBSD Handbook chapter on `Wireless Networking <http://www.freebsd.org/doc//books/handbook/network-wireless.html>`_ provides a good overview of how
wireless works and offers some troubleshooting suggestions.

.. index:: backup
.. _Life Preserver:

Life Preserver
==============

The built-in Life Preserver utility was designed to take full advantage of ZFS snapshot functionality. This utility allows you to schedule snapshots of a
local ZFS pool and to optionally replicate those snapshots to another system using SSH. This design provides several benefits: 

* A snapshot provides a "point-in-time" image of the ZFS pool. In one way, this is similar to a full system backup as the snapshot contains the information
  for the entire filesystem. However, it has several advantages over a full backup. Snapshots occur instantaneously, meaning that the filesystem does not need
  to be unmounted and you can continue to use applications on your system as the snapshot is created. Since snapshots contain the meta-data ZFS uses to access
  files, the snapshots themselves are small and subsequent snapshots only contain the changes that occurred since the last snapshot was taken. This space
  efficiency means that you can take snapshots often. Snapshots also provide a convenient way to access previous versions of files as you can simply browse to
  the point-in-time for the version of the file that you need. Life Preserver makes it easy to configure when snapshots are taken and provides a built-in
  graphical browser for finding and restoring the files within a snapshot.

* Replication is an efficient way to keep the files on two systems in sync. In the case of Life Preserver, the snapshots taken on the PC-BSD system will be
  synchronized with their versions stored on the backup server.

* SSH means that the snapshots will be sent to the backup server oven an encrypted connection, which protects the contents of the snapshots.

* Having a copy of the snapshots on another system makes it possible to restore the pool should your PC-BSD® system become unusable.

If you decide to replicate the snapshots to a backup server, keep the following points in mind when choosing which system to use as the backup server: 

* The backup server **must be formatted with the latest version of ZFS,** also known as ZFS feature flags or ZFSv5000. Operating systems that support this
  version of ZFS include PC-BSD® and FreeBSD 9.2 or higher, and FreeNAS 9.1.x or higher.

* That system must have SSH installed and the SSH service must be running. If the backup server is running PC-BSD, SSH is already installed and you can start
  SSH using :ref:`Service Manager`. If that system is running FreeNAS®, SSH is already installed and how to configure this service is described in
  :ref:`Backing Up to a FreeNAS System`. If the system is running FreeBSD, SSH is already installed, but you will need to start SSH.

* If the backup server is running PC-BSD, you will need to open TCP port 22 (SSH) using :ref:`Firewall Manager`. If the server is running FreeBSD and a
  firewall has been configured, add rules to open this port in the firewall ruleset. FreeNAS® does not run a firewall by default.

.. index:: backup
.. _Scheduling a Backup:

Scheduling a Backup
-------------------

An icon to the Life Preserver utility, seen in Figure 8.19a, can be found in the system tray.

**Figure 8.19a: Life Preserver Icon in System Tray** 

.. image:: images/lpreserver1.png

If you right-click this icon, the following options are available:

* **Open Life Preserver:** starts Life Preserver, which will prompt for your password. The first time this utility is run, it will display the screen shown in
  Figure 8.19b.

* **View Messages:** opens a pop-up menu to display information messages and the Life Preserver log file. There won't be any until after Life Preserver is
  configured and starts to create backups. If a configured Life Preserver task fails, such as replication, check the "Log Files" tab.

* **Popup Settings:** configures Life Preserver to display its notifications over the Life Preserver icon. Choices are "Show all", which includes
  notifications for when a backup/replication starts or finishes, "Warnings only", which only displays warning and error messages, and "None" which disables
  pop-up notification. The default setting is "Warnings only". This setting does not affect critical messages which will always open a window that requires
  attention.

* **Refresh Tray:** by default, Life Preserver checks the status of the pool and available disk space every 30 minutes. Click this option to check the current
  status of a scrub or to check the current disk space after removing a snapshot.

* **Close Tray:** will remove the icon from the system tray. To re-add it to the tray, go to :menuselection:`Control Panel --> Life Preserver` or type
  :command:`life-preserver-tray &` at the command line. If your desktop manager does not provide a system tray, you will need to instead refer to the section
  :ref:`Using the lpreserver CLI`. 

The status of this icon changes when a snapshot, scrub, or replication task is occurring. If you hover over the icon during this time, a tooltip will describe
the current status of the running task.

**Figure 8.19b: Initial Life Preserver Screen**

.. image:: images/lpreserver2.png

To create a backup schedule, click :menuselection:`File --> Manage Pool` and select the name of the pool to manage. The following examples are for a pool
named *tank*. This will launch the the "New Life Preserver Wizard", allowing you to configure the backup schedule. Click "Next" to see the screen in Figure
8.19c. 

**Figure 8.19c: Snapshot Schedule Screen**

.. image:: images/lpreserver3.png

This screen is used to schedule how often a snapshot is taken of the system. The default of "Automatic" creates a snapshot every 5 minutes, which are kept for
an hour, then the hourly is kept for 24 hours, the daily is kept for a month, and the monthly is kept for a year. You can change this default to take one
daily snapshot at a specified time or to take a snapshot once every hour, 30 minutes, 10 minutes or 5 minutes.

If you change the default of "Automatic", the next screen, shown in Figure 8.19d, is used to configure how long to keep the snapshots.

**Figure 8.19d: Snapshot Pruning Screen**

.. image:: images/lpreserver4.png

Snapshots can be configured to be pruned after the specified number of days or after the specified number of snapshots. This screen will not appear for
"Automatic" as it has its own pruning schedule, as explained in the previous paragraph.

.. note:: auto-pruning only occurs on the snapshots generated by Life Preserver according to the configured schedule. Auto-pruning will not delete any
   snapshots you create manually from the "Snapshots" tab.

After making your selection, press "Next" to see the screen shown in Figure 8.19e.

**Figure 8.19e: Replication Server Screen**

.. image:: images/lpreserver5.png

If you wish to keep a copy of the snapshots on another system, this screen is used to indicate which system to send the snapshots to.
If you have another system available which is running the same version of ZFS and has SSH enabled, click the "Replicate my data" box, then input the
following information. **Before entering the information in these fields, you need to first configure the backup system**. An example configuration is
demonstrated in :ref:`Backing Up to a FreeNAS System`.

* **Host Name:** of the remote system that will store your backup. If the backup server is on your local network, the host name must be in your hosts file or
  in the database of the local DNS server. You may find it easier to instead input the IP address of the backup server as this will eliminate any host name
  resolution problems.

* **User Name:** this user must have permission to log in to the system that will hold the backup. If the account does not already exist, you should create it
  first on the backup server.

* **SSH Port:** port 22, the default port used by SSH is selected for you. You only need to change this if the remote system is using a non-standard port to
  listen for SSH connections. In that case, use the up/down arrows or type in the port number.

* **Remote Dataset:** input the name of an existing ZFS dataset on the backup server. This is where the backups will be stored. To get a list of existing
  datasets, type :command:`zfs list` on the remote server. The "NAME" column in the output of that command gives the fullname of each dataset. Type the
  fullname of the desired dataset into this field. When selecting a dataset, make sure that the selected "User Name" has permission to write to the dataset.

* **Frequency:** snapshots can either be sent the same time that they are created or you can set a time or the schedule when the queued snapshots are sent.

When finished, click "Next" to see the screen shown in Figure 8.19f.

**Figure 8.19f: Scrub Schedule Screen**

.. image:: images/lpreserver19.png

This screen is used to schedule when a ZFS scrub occurs. Scrubs are recommended as they can provide an early indication of a potential disk failure. To schedule the scrub,
check the box "Enable scheduled scrub" which will activate the configurable options in this screen. Use the drop-down menu to select a frequency of "Daily",
"Weekly", or "Monthly". If you select "Daily", you can configure the "Hour". If you select "Weekly", you can configure the "Day of week" and the "Hour". If you
select "Monthly", you can configure the "Day of month", "Day of week", and "Hour". Since a scrub can be disk I/O intensive, it is recommended to pick a time when
the system will not be in heavy use. When you are finished, click "Finish". If you configured replication, Life Preserver will check that it can connect to the backup
server and will prompt for the password of "User Name". A second pop-up message will remind you to save the SSH key to a USB stick (as described below) as this key is required
for :ref:`Restoring the Operating System`.

.. note:: if you don't receive the pop-up message asking for the password, check that the firewall on the backup system, or a firewall within the network, is
   not preventing access to the configured "SSH Port".

Once the wizard completes, a system snapshot will be taken. If you also configured replication, Life Preserver will begin to replicate that snapshot to the
remote system. Note that the first replication can take several hours to complete, depending upon the speed of the network. Subsequent replications will only
have changed data and will be much smaller.

Life Preserver uses backend checks so that it is safe to keep making snapshots while a replication is in process. It will not prune any existing snapshots
until the replication is finished and it will not start a second replication before the first replication finishes.

The rest of this section demonstrates the tasks that can be performed from the Life Preserver GUI now that the pool has an initial configuration.

.. index:: backup
.. _Life Preserver Options:

Life Preserver Options
----------------------

Once the schedule for *tank* has been created, the "Status" tab shown in Figure 8.21f will become active and will show the current state of the pool. The
"View" menu lets you select "Basic" or "Advanced" view. "Advanced" view has been selected in the example shown in Figure 8.19g. 

**Figure 8.19g: Life Preserver in Advanced View**

.. image:: images/lpreserver6.png

In this example, the ZFS pool is active, is comprised of one disk, and the date and time of the last snapshot is displayed. The green status indicates that
the latest scheduled replication was successful.

If you click the "Configure" button, the screen shown in Figure 8.19h will open. This allows you to modify the schedule and pruning options in the "Local Snapshots" tab,
the replication settings in the "Replication" tab, and the scrub schedule in the "Scrub" tab.

**Figure 8.19h: Modifying the Configuration**

.. image:: images/lpreserver7.png

The "Restore Data" tab, seen in Figure 8.19i, is used to view the contents of the local snapshots and to easily restore any file which has since been modified
or deleted.

**Figure 8.19i: Viewing the Contents of the Snapshots**

.. image:: images/lpreserver8.png

In this example, the system has been configured to make a snapshot every 5 minutes. Since files have been modified on this system, the blue time slider bar
indicates that several snapshots are available as a snapshot only occurs if changes have been made within the scheduled time increment. Click the arrows to go
back or forward one snapshot at a time. Alternately, click the slider until you are viewing the desired time of the snapshot.

Once you have selected the desired date and time, use the drop-down menu to select the portion of the filesystem to view. In this example, the user has
selected :file:`/usr/home/dru` as that is the user's home directory. The user can now expand the directory names to view the files within each directory.

If your intent is to restore an earlier version of a file or a file that has been deleted, go back to the desired date and time, highlight the file, and click
the "Restore" button. A copy of that file as it appeared at that point in time will be created in the same directory, with :file:`-reversion#` added to the
filename. This way, any current version or restored version of the file will never be overwritten.

The "File" menu contains the following options: 

* **Manage Pool:** this will be greyed out if you have already configured your ZFS pool. If you have a second ZFS pool, you can select this option in order to
  start the Life Preserver Configuration Wizard for that pool.

* **Unmanage Pool:** if you wish to disable ZFS snapshots, select the ZFS pool name. Pop-up menus will ask if you are sure and then ask if you also want to
  delete the local snapshots from the system. If you choose to delete these snapshots, you will lose all of the older versions of the files contained in those
  backups. Once you have unmanaged a pool, you will need to use "Manage Pool" to rerun the Life Preserver Configuration Wizard for that pool.

* **Enable Offsite Backups:** used to configure fully-encrypted backups, where the data is stored as encrypted on the backup server. Refer to
  :ref:`Configuring Encrypted Backups` for instructions.

* **Save Key to USB:** when you configure the replication of local snapshots to a remote system, you should immediately copy the automatically generated SSH
  key to a USB stick. Insert a FAT32 formatted USB stick and wait for :ref:`Mount Tray` to mount it. Then, click this option to copy the key.

* **Close Window:** closes the Life Preserver window. However, Life Preserver will continue to reside in the system tray.

The "Classic Backups" menu can be used to create an as-needed tarball of the user's home directory. This can be handy if you would like to make a backup of just your home
directory in order to restore it in another directory or on another system.

To make a tar backup, click :menuselection:`Classic Backups --> Compress Home Dir` and select the name of the user. This will open the screen shown in Figure
8.19j. 

**Figure 8.19j: Backing Up a User's Home Directory**

.. image:: images/lpreserver9.png

If you want, you can change the name of the archive to create. By default it will be in the format *username-YYYYMMDD-HHMM*. This screen also allows you to
define which files to exclude from the backup. By default, it will exclude PBI shortcuts and, if it exists, the :file:`/bin` directory in the user's home
directory. To exclude a specific file, click the white file icon to browse to the location of the file. To exclude a specific directory, click the blue
directory icon to browse to the location of the directory.

Press "Start" to start the backup. A progress bar will indicate the status and size of the backup. Once the backup is complete, click the "Finished" button to
exit this screen.

Use :menuselection:`Classic Backups --> Extract Home Dir` to restore a previously made home directory backup.
**Be sure this is what you want to do before using this option, as it will overwrite the current contents of the user's home directory.** If your goal is to
restore files without destroying the current versions, use the "Restore Data" tab instead.

The "Snapshots" menu allows you to create or delete snapshots outside of the configured snapshot creation and pruning schedules. This tab contains these options: 

* **New Snapshot:** click this button to create a snapshot now, instead of waiting for the schedule. For example, you can create a snapshot before making
  changes to a file, so that you can preserve a copy of the previous version of the file. Or, you can create a snapshot as you make modifications to the
  system or upgrade software. When creating a snapshot, a pop-up message will prompt you to input a name for the snapshot, allowing you to choose a name that
  is useful in helping you remember why you took the snapshot.

* **Delete Snapshot:** selecting this option will display the list of locally stored snapshots, listed in order from the oldest to the newest. If you select a
  snapshot, a warning will remind you that this is a permanent change that can not be reversed. In other words, the versions of files at that point in time
  will be lost.

* **Start Replication:** if you have configured replication to a remote server, select this option and select the IP address of the remote system to start a
  replication now, rather than waiting for the scheduled time.

* **Re-Initialize Replications:** if a replication fails, it may prevent subsequent replications from completing successfully. In this case, select this option and
  select the IP address of the remote system in order to reset replication. After performing this re-initialization, use "Start Replication" to confirm that the replication issue has
  been resolved and snapshots are being replicated.

The "Disks" menu provides the same functionality of :ref:`Mirroring the System to a Local Disk`, but from the GUI rather than the command line. You should read that
section before attempting to use any of the disk options in this menu. It also lets you start and stop a ZFS scrub.

The options available in this menu are: 

* **Attach Disk:** if you wish to mirror another internal disk or an inserted external USB disk, select this option. The disk must be at least the same size
  as the PC-BSD® disk and you must be willing to have that disk be re-formatted.
  **Do not select this option if you have any data on the disk that you wish to keep.** This option is the GUI front-end to the
  :command:`lpreserver zpool attach` command.

* **Detach Disk:** if you wish to remove an attached disk from the mirror, use this option. This option is the GUI front-end to the
  :command:`lpreserver zpool detach` command.

* **Set Disk Online/Offline:** if you need to temporarily disconnect or reconnect an attached external USB drive, select the appropriate option. This is the
  GUI equivalent to running :command:`lpreserver zpool offline` or :command:`lpreserver zpool online`.

* **Start/Stop Scrub:** it is a good idea to regularly perform a ZFS scrub to verify the integrity of the ZFS pool. Typically, a scrub is run once a week or
  before performing operations such as adding more disks to a pool. The status and results of the scrub can be viewed from the command line by typing
  :command:`zpool status`. When viewing the results of the scrub, check to see if there were any errors, as this is typically an early indication of a failing
  disk. If you are getting errors, consider backing up your data and replacing the failing disk. Since a scrub is I/O intensive, it is recommended to start
  the scrub when the system is not in use, such as before going to bed. Depending upon the size of the pool, the scrub may take some time.

.. index:: backup
.. _Using the lpreserver CLI:

Using the lpreserver CLI
------------------------

The :command:`lpreserver` command line utility can be used to manage snapshots and replication from the command line of a PC-BSD® or TrueOS® system. This
command needs to be run as the superuser. To display its usage, type the command without any arguments::

 lpreserver
 Life-Preserver 
 --------------------------------- 
 Available commands 
 Type in help <command> for information and usage about that command
       help - This help file or the help for the specified command
   cronsnap - Schedule snapshot creation via cron
  cronscrub - Shcedule scrub via cron
        get - Get list of lpreserver options
   listcron - Listing of scheduled snapshots
   listsnap - List snapshots of a zpool/dataset
     mksnap - Create a ZFS snapshot of a zpool/dataset
  replicate - Enable / Disable ZFS replication to a remote system
 revertsnap - Revert zpool/dataset to a snapshot
     rmsnap - Remove a snapshot
        set - Set lpreserver options
     status - List datasets, along with last snapshot / replication date 
      zpool - Manage a zpool by attaching / detaching disks

Each command has its own help text that describes its parameters and provides a usage example. For example, to receive help on how to use the
:command:`lpreserver cronsnap` command, type::

 lpreserver help cronsnap
 Life-Preserver
 ---------------------------------
 Help cronsnap
 Schedule a ZFS snapshot
 Usage:
 For a listing of all scheduled snapshots
 # lpreserver listcron
 or
 To start / stop snapshot scheduling
 # lpreserver cronsnap <dataset> <action> <frequency> <numToKeep>
 action = start / stop
 frequency = auto / daily@XX / hourly / 30min / 10min / 5min
                          ^^ Hour to execute

 numToKeep = Number of snapshots to keep total

 NOTE: When Frequency is set to auto the following will take place:
 * Snapshots will be created every 5 minutes and kept for an hour.
 * A hourly snapshot will be kept for a day.
 * A daily snapshot will be kept for a month.
 * A Monthly snapshot will be kept for a year.
 * The life-preserver daemon will also keep track of the zpool disk space,
   if the capacity falls below 75%, the oldest snapshot will be auto-pruned.

 Example:
 lpreserver cronsnap tank1/usr/home/kris start daily@22 10
 or
 lpreserver cronsnap tank1/usr/home/kris stop

Table 8.19a shows the command line equivalents to the graphical options provided by the Life Preserver GUI. Note that some options are only available from the
command line.

**Table 8.19a: Command Line and GUI Equivalents** 

+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **Command Line**  | **GUI**                                       | **Description**                                                                        |
+===================+===============================================+========================================================================================+
| **cronsnap**      | :menuselection:`Configure --> Local Snapshots`| schedule when snapshots occur and how long to keep them; the **stop** option can be    |
|                   |                                               | used to disable snapshot creation                                                      |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **cronscrub**     |                                               | schedule a ZFS scrub                                                                   |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **get**           |                                               | list Life Preserver options                                                            |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **listcron**      |                                               | list which ZFS pools have a scheduled snapshot                                         |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **listsnap**      | Restore Data                                  | list snapshots of specified dataset                                                    |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **mksnap**        | :menuselection:`Snapshots --> New Snapshot`   | create and replicate a new ZFS snapshot; by default, snapshots are recursive, meaning  |
|                   |                                               | that a snapshot is taken of every dataset within a pool                                |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **replicate**     | :menuselection:`Configure --> Replication`    | used to list, add, and remove backup server; read the **help** for this command for    |
|                   |                                               | examples                                                                               |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **revertsnap**    |                                               | revert dataset to the specified snapshot version                                       |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **rmsnap**        | :menuselection:`Snapshots --> Delete Snapshot`| deletes specified snapshot; by default, all datasets within the snapshot are deleted   |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **set**           |                                               | configures Life Preserver options; read **help** for the list of configurable options  |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **status**        | Status                                        | lists the last snapshot name and replication status                                    |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+
| **zpool**         | :menuselection:`Disks --> Attach Disk` and    | used to attach/detach drives from the pool; read **help** for examples                 |
|                   | :menuselection:`Disks --> Detach Disk`        |                                                                                        |
+-------------------+-----------------------------------------------+----------------------------------------------------------------------------------------+

.. index:: backup
.. _Mirroring the System to a Local Disk:

Mirroring the System to a Local Disk 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In addition to replicating to a remote server, the :command:`lpreserver` command also provides a method for attaching a new disk drive to an existing ZFS
pool, and live-mirroring all data to that disk as data changes on the pool. The attached disk drive can be another internal disk or an external USB disk. When
the new disk is attached for the first time, it will be erased and used solely as a mirror of the existing system drive. In addition, it will be made
bootable, allowing you to boot from and use the new disk should the primary disk fail. In order to use this feature you will need the following: 

* an internal or external disk drive that is the same size or larger than the existing system disk.

* since the disk will be formatted, it must be either blank or not have any data that you wish to keep intact.

* in order to boot from the disk should the primary disk fail, the system must support booting from the new disk. For example, if you are using a USB disk,
  make sure that the BIOS is able to boot from a USB disk.

The superuser can setup the new disk using the following command. Replace *tank1* with the name of your ZFS pool and */dev/da0* with the name of the disk to
format. For example, the first USB disk will be */dev/da0* and the second internal hard disk will be */dev/ad1*::

 lpreserver zpool attach tank1 /dev/da0

When the disk is first attached, it will be formatted with ZFS and configured to mirror the size of the existing disk. GRUB will also be stamped on the new
disk, making it bootable should another drive in the array go bad. You can add multiple disks to the pool in this manner, giving any level of redundancy that
you require.

Once the disk is attached, it will begin to resilver. This process mirrors the data from the primary disk to the newly attached disk. This may take a while,
depending upon the speed of the disks and system load. Until this is finished you should not reboot the system, or detach the disk. You can monitor the
resilvering process by typing :command:`zpool status`.

To get a listing of the disks in your mirror, run this command, replacing *tank1* with the name of the pool::

 lpreserver zpool list tank1

If you are using an external drive, there may be occasions where you wish to disconnect the backup drive, such as when using a laptop and going on the road.
In order to so this safely, it is recommended that you first offline the external disk using the following command::

 lpreserver zpool offline tank1 /dev/da0

Then when you re-connect the drive, you can place it in online mode again using::

 lpreserver zpool online tank1 /dev/da0

Sometimes, the disk name will change as a result of being disconnected. The :command:`lpreserver zpool list tank1` command can be used to get the proper
device ID.

If you wish to permanently remove a disk from the mirror, run the following command. If you decide to re-attach this disk later, a full disk copy will again
have to be performed::

 lpreserver zpool detach tank1 /dev/da0

.. note:: in addition to working with mirrors, the :command:`lpreserver zpool` command can also be used to manage a RAIDZ configuration, although you will
   probably not want to use external disks in this case.

.. index:: backup
.. _Backing Up to a FreeNAS System:

Backing Up to a FreeNAS System
------------------------------

`FreeNAS® <http://www.freenas.org/>`_ is an open source Networked Attached Storage (NAS) operating system based on FreeBSD. This operating system is designed
to be installed onto a USB stick so that it is kept separate from the storage disk(s) installed on the system. You can download the latest STABLE version of
FreeNAS® 9.3 from `download.freenas.org <http://download.freenas.org/9.3/STABLE/>`_ and read its documentation at 
`doc.freenas.org <http://doc.freenas.org/9.3/>`_. 

This section demonstrates how to configure FreeNAS® 9.3 as the backup server for Life Preserver to replicate to. It assumes that you have already installed
this version of FreeNAS® using the installation instructions in the
`FreeNAS® 9.3 Users Guide <http://doc.freenas.org/9.3/freenas_install.html>`_ and are able to access the FreeNAS® system from a web browser.

In order to prepare the FreeNAS® system to store the backups created by Life Preserver, you will need to create a ZFS volume, create and configure the
dataset to store the backups, create a user account that has permission to access that dataset, and enable the SSH service.

In the example shown in Figure 8.19j, the user has clicked :menuselection:`Storage --> Volumes --> Volume Manager` in order to create the ZFS volume from the
available drives.

**Figure 8.19j: Creating a ZFS Volume in FreeNAS®** 

.. image:: images/lpreserver10.png

Input a "Volume Name", drag the slider to select the number of available disks, and click the "Add Volume" button. The Volume Manager will automatically
select the optimal layout for both storage capacity and redundancy. In this example, a RAIDZ2 named *volume1* will be created.

To create the dataset to backup to, click the "+" next to the entry for the newly created volume, then click "Create ZFS Dataset". In the example shown in
Figure 8.19k, the "Dataset Name" is *backups*. Click the "Add Dataset" button to create the dataset.

.. note:: make sure that the dataset is large enough to hold the replicated snapshots. To determine the size of the initial snapshot, run
   :command:`zpool list` on the PC-BSD® system and look at the value in the "ALLOC" field. Subsequent snapshots will be smaller and will be the size of the
   data that has changed.

**Figure 8.19k: Creating a ZFS Dataset in FreeNAS®**

.. image:: images/lpreserver11.png

To create the user account, go to :menuselection:`Account --> Users --> Add User`. In the screen shown in Figure 8.19l, input a "Username" that will match the
"User Name" configured in Life Preserver. Under "Home Directory", use the browse button to browse to the location of the dataset that you made to store the
backups. Input a "Full Name", then input and confirm a "Password". When finished, click the "OK" button to create the user.

**Figure 8.19l: Creating a User in FreeNAS®**

.. image:: images/lpreserver12.png

Next, give the user permissions to the dataset by going to :menuselection:`Storage --> Volumes`, click the + next to the name of the volume, click the "+"
next to the name of the dataset, then click "Change Permissions" for the expanded dataset. In the screen shown in Figure 8.19m, change the "Owner (user)"and
"Owner (group)" to the user that you created. Click "Change" to save the change.

**Figure 8.19m: Setting Permissions in FreeNAS®**

.. image:: images/lpreserver13.png

Next, click on "Shell" and type the following command, replacing *dru* and *volume1/backups* with the name of the user, volume, and dataset that you created::

 zfs allow -u dru atime,canmount,clone,compression,create,destroy,hold,mount,mountpoint,promote,receive,rename,send,userprop volume1/backups

Click the "x" in the upper right corner to close "Shell". Then, to enable the SSH service, go to :menuselection:`Services --> Control Services`, shown in
Figure 8.19n. 

**Figure 8.19n: Start SSH in FreeNAS®**

.. image:: images/lpreserver14.png

Click the red "OFF" button next to SSH to enable that service. Once it turns to a blue "ON", the FreeNAS® system is ready to be used as the backup server.

To finish the configuration, go to the PC-BSD® system. If you have not yet configured Life Preserver, in the wizard screen shown in Figure 8.19e, check the
"Replicate my data" box and click the "Scan Network" button. A pop-up menu should show the available systems running SSH in the network so that the "Host Name"
field can be populated from your selection. If you instead receive an error message, check to see if there is a firewall between the PC-BSD® and the FreeNAS® system.
If there is, add a rule to allow UDP port 5353. Alternately, you can manually input the IP address of the FreeNAS® system in the "Host Name" field. Also input the name
of the user you created in the "User Name" field and the name of the dataset you created (in this example it is *volume1/backups)* in the "Remote Dataset" field. You
should be prompted for the user's password and to save a copy of the SSH key to a USB stick.

If the system has already been configured, go to :menuselection:`Configure --> Replication` and click the "+" button to select the hostname of the FreeNAS® system.
If needed, input or correct the information in the "User Name" and the "Remote Dataset" fields and select the desired replication frequency in the "Frequency" drop-down menu.

.. index:: backup
.. _Configuring Encrypted Backups:

Configuring Encrypted Backups
-----------------------------

For some time, Life Preserver has provided the ability to securely replicate to another system over SSH, meaning that the data is encrypted while it is being transferred
over the network. Beginning with version 10.1.2, Life Preserver provides an extra measure of security to replicated backups by adding support for fully-encrypted backups,
using `stunnel <https://www.stunnel.org/index.html>`_ and GELI-backed iSCSI volumes. This means that the data stored on the remote side is encrypted and only accessibly with
the key file stored on the PC-BSD® client. The backup server must understand kernel iSCSI, meaning that it must be running FreeBSD 9.1 or higher, PC-BSD®/TrueOS® 10.1.2, or
FreeNAS® 9.3. However, the remote system does not need to be formatted with ZFS. This section describes how to configure the backup system and how to use the new setup wizard
for creating encrypted backups.

.. _Preparing the Backup System:

Preparing the Backup System
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The backup system must meet the following requirements:

* must be running FreeBSD 9.1 or higher, PC-BSD® or TrueOS® 10.1.2, or FreeNAS® 9.3

* if it is a FreeBSD system, the "security/stunnel" package must be installed; this software is already installed on PC-BSD®/TrueOS® 10.1.2 and on FreeNAS® 9.3 systems that
  have been updated to at least SU201504100216.
  
* if it is a FreeBSD system, the `lpreserver-host-iscsi <https://raw.githubusercontent.com/pcbsd/pcbsd/master/src-sh/lpreserver/lpreserver-host-iscsi>`_ script must be
  downloaded. This file is already installed to :file:`/usr/local/bin/` on PC-BSD®/TrueOS® 10.1.2 systems. See the next section for FreeNAS® instructions.

Before you can configure the PC-BSD® system, you must first create a Life Preserver configuration file ending in the :file:`.lps` extension on the remote system which
will store the encrypted backups. To create this file on a FreeBSD 9.1 or higher system or on a PC-BSD®/TrueOS® 10.1.2 system, run the :command:`lpreserver-host-iscsi`
script as the *root* user. Input the information that the script asks for as seen in the following example. Table 8.19b summarizes the various options that this script prompts for::

 lpreserver-host-iscsi
 Enter the target host name (example.com or IP)
 >10.0.0.1
 Enter the target name (target0)
 > target0
 Enter the CHAP username
 >mybackups
 Enter the CHAP password (12-16 chars)
 >pcbsdbackups
 Enter the ZVOL name (I.E. tank/myzvol)
 >tank/pcbsd-backup
 Enter the ZVOL size (I.E. 800M, 4G, 1T)
 >50G
 Does this look correct?
 Target host: 10.0.0.1
 Target name: target0
 Username: mybackups
 Password: pcbsdbackups
 ZVOL name: tank/pcbsd-backup
 ZVOL size: 50G
 (y/n)>y

Once you input *y*, the script will configure the necessary services for startup, generate an RSA key, and prompt you for information to go into the digital certificate, as seen in
this example::

 ctld_enable: NO -> YES
 stunnel_enable: -> YES
 Generating RSA private key, 2048 bit long modulus
 .............................+++
 ......................................+++
 e is 65537 (0x10001)
 You are about to be asked to enter information that will be incorporated
 into your certificate request.
 What you are about to enter is what is called a Distinguished Name or a DN.
 There are quite a few fields but you can leave some blank
 For some fields there will be a default value,
 If you enter '.', the field will be left blank.
 -----
 Country Name (2 letter code) [AU]: US
 State or Province Name (full name) [Some-State]: CA
 Locality Name (eg, city) []: San Jose
 Organization Name (eg, company) [Internet Widgits Pty Ltd]: My Backups
 Organizational Unit Name (eg, section) []:
 Common Name (e.g. server FQDN or YOUR name) []: Dru
 Email Address []:
 ctld not running? (check /var/run/ctld.pid).
 Starting ctld.
 stunnel not running?
 Starting stunnel.
 Created mybackups.lps

Once you have successfully created the :file:`.lps` file and copied it to the PC-BSD® system, you are ready to configure the PC-BSD® system using the instructions in
:ref:`Running the Encrypted Backup Wizard`.

**Table 8.19b: Configuration Options** 

+------------------+---------------------------------------------------------------------------------------------------------------------------+
| **Option**       | **Description**                                                                                                           |
+==================+===========================================================================================================================+
| target host name | the IP address of the server which will hold the encrypted backups                                                        |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+
| target name      | can be anything, as long as it is does not already exist in :file:`/etc/ctl.conf`                                         |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+
| CHAP username    | must be between 8 and 12 characters                                                                                       |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+
| CHAP password    | must be between 12 and 16 characters                                                                                      |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+
| ZVOL name        | in the format *poolname/something-useful*                                                                                 |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+
| ZVOL size        | **must be at least the same size as the pool to be backed up**                                                            |
|                  |                                                                                                                           |
+------------------+---------------------------------------------------------------------------------------------------------------------------+

.. _Using FreeNAS as the Backup System:

Using FreeNAS as the Backup System
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To instead prepare a FreeNAS® 9.3 system to use as the backup target, first ensure that the system has been updated to the latest software update. Then,
perform the following configuration steps.

Create a service account for the stunnel service by going to :menuselection:`Account --> Users --> Add User`. In the screen shown in Figure 8.19o, input
the following values in these fields then press "OK" to create the account:

* **User ID:** 341

* **Username:** stunnel

* **Shell:** nologin

* **Full Name:** stunnel service (or something useful to you)

* **Disable password login:** check this box

**Figure 8.19o: Create the Service Account** 

.. image:: images/iscsi4.png

Next, create a zvol by using the tree menu to go to :menuselection:`Storage --> Volumes --> click the plus to expand name of volume -> Create zvol`. In the example
shown in Figure 8.19p, a zvol of 50GB in size named "pcbsd-backup" is created on the volume named "volume1".

**Figure 8.19p: Create the zvol** 

.. image:: images/iscsi5.png

You are now ready to configure iSCSI. Go to :menuselection:`Sharing --> Block (iSCSI)`. In the "Target Global Configuration" screen shown in Figure 8.19q, change the
default "Base Name" to *iqn.2012-06.com.lpreserver*.

**Figure 8.19q: Configure the IQN** 

.. image:: images/iscsi6.png

Click the "Portals" tab then the "Add Portal" button. Verify that the "IP Address" drop-down menu is set to *0.0.0.0* and that the "Port" field is set to
*3260*, add a "Comment" if it is useful to you, then click "OK" in order to add an entry to the "Portals" tab. In the example shown in Figure 8.19r, this is
the first time iSCSI has been configured on this system, so it has a "Portal Group ID" of *1*. If you have already created other iSCSI targets, note the
"Portal Group ID" you just created.

**Figure 8.19r: Configure the Portal** 

.. image:: images/iscsi7.png

In the "Initiators" tab, click the "Add Initiator" button. Verify that both the "Initiators" and "Authorized network" fields are set to *ALL*, add a "Comment" if
it is useful to you, and press "OK" in order to add an entry to the "Initiators" tab. Make note of the "Group ID" that is created. In the example shown in Figure 8.19s,
it is *1*.

**Figure 8.19s: Configure the Initiator** 

.. image:: images/iscsi8.png

In the "Authorized Access" tab, click the "Add Authorized Access" button. Input a value for the "User" that is between 8 and 12 characters and the "Secret" and
"Secret (Confirm)" fields that is between 12 and 16 characters, then press "OK". In the example shown in Figure 8.19t, the "User" has a value of *mybackups*, the
secret is *pcbsdbackups*, and the "Group ID" is
*1*. Make note of the "Group ID" that is created for you.

**Figure 8.19t: Configure the Authorized Access** 

.. image:: images/iscsi9.png

In the "Targets" tab, click the "Add Target" button. In the screen shown in Figure 8.19u, use the following values in these fields:

* **Target Name:** target0

* **Portal Group ID:** select the group ID you created in the drop-down menu

* **Initiator Group ID:** select the group ID you created in the drop-down menu

* **Auth Method:** select CHAP from the drop-down menu

**Figure 8.19u: Configure the Target** 

.. image:: images/iscsi10.png

In the "Extents" tab, click the "Add Extent" button. In the screen shown in Figure 8.19v, input an "Extent Name", in this case it is *pcbsd-backup* and make sure that
the zvol you created is selected in the "Device" drop-down menu. Click "OK" to create the extent.

**Figure 8.19v: Configure the Extent** 

.. image:: images/iscsi11.png

Finish the iSCSI configuration by clicking the "Associated Targets" tab, then the "Add Target / Extent" button. In the screen shown in Figure 8.19w, select the "Target"
and the "Extent" that you created.

**Figure 8.19w: Associate the Target With the Extent** 

.. image:: images/iscsi12.png

Next, go to :ref:`Services` and click the red "OFF" next to the iSCSI service. Wait for it to turn to a blue "ON", indicating that the iSCSI service has started.

To configure the stunnel service, open :ref:`Shell` and type the following::

 openssl genrsa -out /usr/local/etc/stunnel/key.pem 2048
 openssl req -new -x509 -key /usr/local/etc/stunnel/key.pem -out /usr/local/etc/stunnel/cert.pem -days 1095
 cat /usr/local/etc/stunnel/key.pem /usr/local/etc/stunnel/cert.pem >> /usr/local/etc/stunnel/iscsi.pem

Next, create a file named :file:`/usr/local/etc/stunnel/stunnel.conf` with the following contents::

 setuid = stunnel
 setgid = nogroup
 options = NO_SSLv2
 client = no

 [iscsi]
 accept = 9555
 connect = 127.0.0.1:3260
 cert = /usr/local/etc/stunnel/iscsi.pem

Make sure that the stunnel service starts using this command::

 service stunnel onestart

.. note:: to ensure that the stunnel service also starts whenever the FreeNAS® system reboots, use :menuselection:`System --> Tunables --> Add Tunable` to create a tunable with
   a "Variable" of *stunnel_enable*, a "Value" of
   *YES*, and a "Type" of
   *rc.conf*.

Finally, create a :file:`*.lps` file on the PC-BSD® system using a text editor. Edit the following example so that the IP address of the FreeNAS® system is in the
"ihost" field, the "iuser" value matches the "User" in :menuselection:`Sharing --> Block (iSCSI) --> Authorized Access`, the "ipassword" value matches the "Secret" you
set when you created the "Authorized Access", and the "itarget" value matches the "Target Name" in :menuselection:`Sharing --> Block (iSCSI) --> Targets`::

 % more mybackups.lps
 [Life-Preserver LPS]
 ihost: 10.0.0.1
 iuser: mybackups
 ipassword: pcbsdbackups
 itarget: target0

You are now ready to configure the PC-BSD® system using the instructions in :ref:`Running the Encrypted Backup Wizard`.

.. _Running the Encrypted Backup Wizard:

Running the Encrypted Backup Wizard
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Once you have configured the backup system and the PC-BSD® system has a copy of the :file:`*.lps` file, you are ready to run the encrypted backup wizard on the
PC-BSD® system. If you have not yet managed a pool in Life Preserver, click :menuselection:`File --> Manage Pool` and follow the initial configuration wizard
described in :ref:`Scheduling a Backup`. When you get to the screen shown in Figure 8.19e, click "Next" as you will be using a zvol rather than a dataset to
backup to. Next, start the encrypted backup wizard by clicking :menuselection:`File --> Enable Offsite Backups` and select the pool to backup. This will start
the "iSCSI Setup Wizard". Click "Next" to see the screen shown in Figure 8.19x.

**Figure 8.19x: Selecting the Configuration File** 

.. image:: images/iscsi1.png

Click the "Select" button to browse to the location of your saved :file:`.lps` file. Once selected, the "Host", "Target", "User", and "Password" fields will
auto-populate with the settings from the configuration file. Click "Next" to see the screen shown in Figure 8.19y.

**Figure 8.19y: Input the Encryption Key** 

.. image:: images/iscsi2.png

This screen lets you configure the following:

* **Backup Schedule:** choices are "On New Snapshot", "Manually Started", "Daily", "Hourly", "Every 30 Minutes", or "Every 10 Minutes". If you select "Daily",
  another field will appear so that you can set the time.

* **Re-use existing data on the host:** by default, this box is unchecked as the encryption key will be automatically generated for you and stored in
  :file:`/var/db/lpreserver/keys`. If you have configured an encrypted backup to the remote backup system in the past and still have a copy of the encryption key for that
  system, check this box and use the browse button to add the key to the "GELI Encryption File" field.
  
When finished, click "Next". A pop-up menu will ask if you are ready to enable off-site data storage. Click "Yes" to complete the configuration. This may take a few minutes.
Once the connection to the remote system is established, you will see the screen shown in Figure 8.19z.

**Figure 8.19z: Save the Key** 

.. image:: images/iscsi3.png

.. _Restoring the Operating System:

Restoring the Operating System
------------------------------

If you have replicated the system's snapshots to a remote backup server, you can use a PC-BSD® installation media to perform an operating system restore or to clone
another system. Start the installation as usual until you get to the screen shown in Figure 8.19aa. 

**Figure 8.19aa: Selecting to Restore/Clone From Backup** 

.. image:: images/lpreserver15.png

Before you can perform a restore, the network interface must be configured. Click the "network connectivity" icon (second from the left) in order to determine
if the network connection was automatically detected. If it was not, refer to :ref:`Network Configuration` before continuing.

Next, click "Restore from Life-Preserver backup" and the "Next" button. This will start the Restore Wizard. Click "Next" to select the type of restore using the
screen shown in Figure 8.19ab. 

**Figure 8.19ab: Restoring From an Encrypted Backup** 

.. image:: images/lpreserver16.png

If you configured an encrypted backup using the instructions in :ref:`Configuring Encrypted Backups`, click the "+" button in the "Encrypted iSCSI Restore" tab to
browse to the location of the :file:`*.lpiscsi` file, enter the password to decrypt this file, and click "Next".

If you instead configured backups to a replication server using the instructions in :ref:`Scheduling a Backup`, click the "SSH Restore" tab. In the screen shown in
Figure 8.19ac, input the IP address of the backup server and the name of the user account used to replicate the snapshots. If the server is listening on a non-standard SSH
port, change the "SSH port" number. Then, click "Next" to select an authentication method in the screen shown in Figure 8.19ad.

**Figure 8.19ac: Input the Information for a SSH Restore** 

.. image:: images/lpreserver20.png

**Figure 8.19ad: Select the Authentication Method** 

.. image:: images/lpreserver17.png

If you previously saved the SSH key to a USB stick, insert the stick then press "Next". Otherwise, change the selection to "Use password authentication" and
press "Next". The next screen will either read the inserted USB key or prompt for the password, depending upon your selection. The wizard will then attempt a
connection to the server.

Once the connection to the backup server succeeds, you will be able to select which host to restore. In the example shown in Figure 8.19ae, only one host has been backed up to the
replication server.

**Figure 8.19ae: Select the Host to Restore**

.. image:: images/lpreserver18.png

After making your selection, click "Next". The restore wizard will provide a summary of which host it will restore from, the name of the user account
associated with the replication, and the hostname of the target system. Click "Finish" and the installer will proceed to the :ref:`Disk Selection Screen`. At
this point, you can click the "Customize" button to customize the disk options. However, in the screen shown in Figure 3.3h, the ZFS datasets will be greyed
out as they will be recreated from the backup during the restore. Once you are finished any customizations, click "Next" to perform the restore.

.. index:: bug
.. _Report a bug:

Report a bug
============

The bug reporting tool in Control Panel can be used to easily send a bug report to the development team responsible for the software which produced the bug.

To access this tool, go to :menuselection:`Control Panel --> Report a bug` or type :command:`pc-bugreport` from the command line.
The initial screen for this tool is shown in Figure 8.20a. 

**Figure 8.20a: PC-BSD® Bug Reporting Utility** 

.. image:: images/report1.png

Select the software component that most closely matches where the bug occurs. For example, if the bug occurs when using a KDE utility, select "Desktop
environment", or if the bug occurs when using an application that was installed using AppCafe®, select "PC-BSD software (pbi)". When in doubt, select
"PC-BSD base system". 

In the example shown in Figure 8.20b, the user has selected "PC-BSD base system" then "Next". 

.. note:: regardless of the selection, the resulting screen will be similar to 8.20b. The various screens only differ in which bug tracking system or mailing
   list is used by the development team for that component. If you select "Desktop environment" you will also be asked to indicate which desktop so that the
   correct information is displayed for that development team. Similarly, if you select "PBI software" you will be asked to select which PBI produces the
   error.

**Figure 8.20b: Reporting a Bug** 

.. image:: images/report2.png

If the development team has a bug tracker, its URL will be displayed. If you click the "Launch web browser" button, that website will be opened in the default
web browser so that you can search for existing bugs and create a new bug if one does not already exist. Note that you will need to register first if this is
your first bug and that you must be logged in in order to create a new bug.

If the development team has a mailing list, its email address will be listed. The URL to the mailing list will also be displayed so that you can search its
archives and subscribe to the list. Note that you will need to be subscribed to a mailing list before you can report a bug on that list. To report the bug,
click the "Compose email" button to open the default mail application. To subscribe to or read the archives of the list, click the "Launch web browser"
button.

The three icons in the "Toolbox" section can be used to gather useful information to include in your bug report. If you click the first icon on the left, a
pop-up menu allows you to create any of the following: 

* Diagnostic report 

* FreeBSD version (:command:`uname -a`) 

* :command:`dmesg` output 

* Xorg version 

* Xorg log 

If you click an entry in the menu, the results will be displayed in a window so that you can copy the contents into your bug report.

If you click the second icon, it will generate a PCI devices list. This is useful information if your built-in wireless card is not working.

If you click the third icon, the default snapshot utility for the desktop will open so that you can include a snapshot in your bug report.

While this utility makes it easy to find the correct place to send a bug report, it is still up to you to make sure that your report includes the information
that developers need to recreate and eventually fix the bug. The following resources contain useful tips for the various development teams for the bugs you
may encounter when using PC-BSD®: Before reporting:

* a bug about the "FreeBSD base system" or "FreeBSD software (ports/packages)", read through 
  `the FreeBSD bug reports page <http://www.freebsd.org/support/bugreports.html>`_. 

* a bug about the "Xorg server", read through the `Xorg FAQ <http://www.x.org/wiki/FAQ/>`_. 

* a KDE bug, read through `how to file a bug-free report in KDE <http://www.muktware.com/5457/how-file-bug-free-bug-report-kde>`_. 

* a GNOME bug, read through `how to file a bug in GNOME's bugzilla <http://askubuntu.com/questions/43487/how-to-file-a-bug-on-gnomes-bugzilla>`_. 

* a LXDE bug, read through `read this first <http://forum.lxde.org/viewtopic.php?t=575>`_.

* a XFCE bug, read through `XFCE bug reporting <http://docs.xfce.org/contribute/bugs/start>`_. 

.. index:: jails
.. _Warden®:

Warden®
========

Warden® is an easy to use, graphical `jail <http://en.wikipedia.org/wiki/FreeBSD_jail>`_ management program. Using Warden®, it is possible to create
multiple, isolated virtual instances of FreeBSD which can be used to run services such as Apache, PHP, or MySQL in a secure manner. Each jail is considered to
be a unique FreeBSD operating system and whatever happens in that jail will not affect your operating system or other jails running on the PC-BSD® system.

Some of the features in Warden® include the ability to: 

* create three types of jails: a traditional FreeBSD jail for running network services, a (less secure) ports jail for safely installing and running FreeBSD
  ports/packages from your PC-BSD® system, and a Linux jail for installing Linux 

* set multiple IPv4 and IPv6 addresses per jail 

* quickly install common network server applications on a per-jail basis 

* update installed software on a per-jail basis 

* manage user accounts on a per-jail basis 

* manage ZFS snapshots on a per-jail basis

* export a jail which can be then be imported into the same or a different jail 

.. index:: jails
.. _Creating Jails:

Creating Jails
--------------

Warden® can be started by clicking on its icon in Control Panel or by typing :command:`pc-su warden gui` from the command line. You will be prompted for your
password as administrative access is needed to create and manage jails. The initial Warden® configuration screen is shown in Figure 8.21a. 

**Figure 8.21a: Initial Warden® Screen**

.. image:: images/warden1.png

To create your first jail, click the "New Jail" button or go to :menuselection:`File --> New Jail`. A jail creation wizard, seen in Figure 8.21b, will launch.

**Figure 8.21b: Creating the New Jail** 

.. image:: images/warden2.png

The first screen in the jail creation wizard will prompt you for the following information: 

**Hostname:** you can change the default of “Jailbird” to another value. The hostname must be unique on your network and can not contain a space. Use a
hostname that reminds you of the type of jail and your reason for creating it.

**IPV4 Address:** if you plan to access the jail and its contents using IPv4, input the IPv4 address to be used by the jail and access its contents. Choose an
address on your network that is not already in use by another computer or jail and which will not conflict with the address range assigned by a DHCP server.

**IPv6 Address:** if you plan to access the jail and its contents using IPv6, check the “IPv6 Address box and input an IPv6 address that is not already in
use by another computer or jail on your network.

When finished, click "Next" to select the type of jail, as shown in Figure 8.21c: 

**Figure 8.21c: Select the Type of Jail** 

.. image:: images/warden3.png

There are three types of jails supported by Warden®: 

**Traditional Jail:** select this type if you are creating the jail in order to install and run network services. For example, this type of jail is
appropriate if you wish to run a web server or a database which is accessible to other systems on a network or over the Internet. This is the most secure type
of jail as it is separate from the PC-BSD® host and any other jails that you create using Warden®. By default, FreeBSD's next generation of package
management, known as pkgng, and the command line versions of the PC-BSD® utilities are added to a default FreeBSD installation. If you do not plan to use
these tools, uncheck the box "Install PKGNG and PC-BSD utilities". If you have already created a jail template using :ref:`Template Manager`, select the
desired operating system version from the "Jail Version" drop-down menu.

**Ports Jail:** select this type of jail if your intention is to install software using FreeBSD packages and ports and you wish to have access to that
software from your PC-BSD® system or if you plan to install any GUI applications within the jail. This type of jail is less secure then a traditional jail as
applications are shared between the jail and the PC-BSD® system. This means that you should not use this type of jail to install services that will be
available to other machines over a network.

**Linux Jail:** select this type of jail if you would like to install a Linux operating system within a jail. Linux jail support is considered to be
experimental and is limited to 32-bit.

The remaining screens will differ depending upon the type of jail that you select.

If you select "Traditional Jail", you will be prompted to set the root password as seen in Figure 8.21d. Input and confirm the password then press "Next" to
see the screen shown in Figure 8.21e. If you instead select to create a "Ports Jail", you will go directly to Figure 8.21e. 

**Figure 8.21d: Setting the Traditional Jail's Root Password** 

.. image:: images/warden4.png

**Figure 8.21e: Select the Jail Options** 

.. image:: images/warden5.png

This screen allows you to install the following options: 

**Include system source:** if you check this box, make sure that :file:`/usr/src/` exists on the PC-BSD system as the source is copied to the jail from this
location. If it is not installed, use :menuselection:`Control Panel --> System Manager --> Tasks --> Fetch PC-BSD System Source` to install it.

**Include ports tree:** if you check this box, the latest version of the ports tree will be downloaded into :file:`/usr/ports/` of the jail. This will allow
you to compile FreeBSD ports within this jail.

**Start jail at system bootup:** if this box is checked, the jail will be started (become available) whenever you boot your main system. If the box is not
checked, you can manually start the jail whenever you wish to access it using Warden®. 

Once you have made your selections, click the "Finish" button to create the jail. Warden® will display a pop-up window containing status messages as it
downloads the files it needs and creates and configures the new jail.

Once Warden® is finished creating the jail, a message should appear at the bottom of the pop-up window indicating that the jail has been successfully
created. Click the "Close" button to return to the main screen.

If you select the "Linux Jail" and click "Next", you will be prompted to set the root password as seen in Figure 8.21d. After inputting the password, the
wizard will prompt you to select a Linux install script, as seen in Figure 8.201. 

**Figure 8.201: Select the Linux Distribution to Install** 

.. image:: images/warden6.png

The installation script is used to install the specified Linux distribution. At this time, installation scripts for Debian Wheezy and Gentoo are provided.

.. note:: a Linux installation script is simply a shell script which invokes a Linux network installation. In the case of Debian Wheezy, it invokes the
   :command:`debootstrap` command.

Once you select the install script, the wizard will ask if you would like to start the jail at boot time as seen in Figure 8.21g. 

**Figure 8.21g: Linux Jail Options** 

.. image:: images/warden7.png

Click the "Finish" button to begin the Linux installation.

.. index:: jails
.. _Managing Jails:

Managing Jails
--------------

Once a jail is created, an entry for the jail will be added to the "Installed Jails" box and the tabs within Warden® will become available. Each entry
indicates the jail's hostname, whether or not it is currently running, and whether or not any updates are available for the meta-packages installed within the
jail. The buttons beneath the "Installed Jails" box can be used to start/stop the highlighted jail, configure the jail, add a new jail, or delete the
highlighted jail.

If you highlight a jail and click “Jail Configuration”, the screen shown in Figure 8.21h will open.

**Figure 8.21h: Jail Configuration Options** 

.. image:: images/warden8.png

The "Options" tab has one checkbox for enabling or disabling VNET/VIMAGE support. This option provides that jail with its own, independent networking stack.
This allows the jail to do its own IP broadcasting, which is required by some applications. However, it breaks some other applications. If an application
within a jail is having trouble with networking, try changing this option to see if it fixes the issue.

The IPv4 tab is shown in Figure 8.21i. 

**Figure 8.21i: Jail IPv4 Options** 

.. image:: images/warden9.png

This screen allows you to configure the following: 

**IPv4 Address:** uncheck this box if you do not want the jail to have an IPv4 address.

**IPv4 Bridge Address (Requires VNET):** if this box is checked, an IP address is input, and the "IPv4 Default Router" box is left unchecked, the bridge
address will be used as the default gateway for the jail. If the "IPv4 Default Router" address is also configured, it will be used as the default gateway
address and the bridge address will be used as just another address that is configured and reachable. This option requires the "Enable VNET/VIMAGE support"
checkbox to be checked in the "Options" tab.

**IPv4 Default Router:** check this box and input an IP address if the jail needs a different default gateway address than that used by the PC-BSD® system.
This option requires the "Enable VNET/VIMAGE support" checkbox to be checked in the Options tab.

The IPv6 tab is shown in Figure 8.21j.

**Figure 8.21j: Jail IPv6 Options** 

.. image:: images/warden10.png

This screen allows you to configure the following: 

**IPv6 Address:** check this box if you want the jail to have an IPv6 address.

**IPv6 Bridge Address (Requires VNET):** if this box is checked, an IPv6 address is input, and the "IPv6 Default Router" box is left unchecked, the bridge
address will be used as the default gateway for the jail. If the "IPv6 Default Router" address is also configured, it will be used as the default gateway
address and the bridge address will be used as just another address that is configured and reachable. This option requires the "Enable VNET/VIMAGE support"
checkbox to be checked in the "Options" tab.

**IPv6 Default Router:** check this box and input an IPv6 address if the jail needs a different default gateway address than that used by the PC-BSD® system.
This option requires the "Enable VNET/VIMAGE support" checkbox to be checked in the "Options" tab.

The "Aliases" tab is shown in Figure 8.21k. 

**Figure 8.21k: Jail Aliases Options** 

.. image:: images/warden11.png

Click the drop-down menu to see all of the options shown in Figure 8.21k. An alias allows you to add additional IP addresses to an interface. Select the type
of address you would like to add an alias to, click the "Add" button, type in the IP address to add and click "OK".

The "Permissions" tab is shown in Figure 8.21l. This screen can be used to easily enable or disable the :command:`sysctl` values that are available for jails.

**Figure 8.21l: Jail Permissions** 

.. image:: images/warden12.png

The "Info" tab, as seen in the example in Figure 8.21m, provides an overview of a jail's configuration. If you have created multiple jails, the "Info" tab
displays the configuration of the currently highlighted jail.

**Figure 8.21m: Info Tab of Warden®** 

.. image:: images/warden13.png

In the example shown in Figure 8.21m, three jails have been created: a traditional jail, a ports jail, and Debian Squeeze has been installed into a Linux
jail.

The "Info" tab contains the following information: 

- **Jail Type:** will indicate if the jail is a Traditional, Ports, or Linux jail.
  
- **Size on Disk:** indicates the amount of space being used by the jail. The jail itself takes up about 300MB of space, source is about 300MB, and ports are
  about 850MB.

- **Start at boot:** a status of "Enabled" indicates that the jail will automatically start when the system reboots. "Disabled" means that you will manually
  start the jail as needed.

- **Active Connections:** will list the number of active connections to the jail (e.g. through :command:`ssh` or one of the running services).

- **IPs**: lists the jail's IP address as well as any configured aliases.

- **Listening on Ports:** indicates which ports are currently listening for connections.

You can sort the jail listing by clicking on the "Jail", "Status", or "Updates" header name. The "Updates" column will indicate if a software or system update
is available for a jail.

The "Tools" tab, shown in Figure 8.21n, allows you to manage common configuration tasks within a jail.

.. note:: make sure that the desired jail is highlighted when using the "Tools" tab.

**Figure 8.21n: Tools Tab for the Highlighted Jail** 

.. image:: images/warden14.png

This tab provides the following buttons: 

- **User Administrator:** opens :ref:`User Manager` so that you can manage the highlighted jail's user accounts and groups. The title bar will indicate that
  you are "Editing Users for Jail: Jailname". Note that any users and groups that you have created on your PC-BSD® system will not be added to a traditional
  jail as each traditional jail has its own users and groups. However, a ports jail has access to the users and groups that exist on the PC-BSD® system, yet
  the users you create on a ports jail will only be available within the ports jail. If you add any user accounts to the PC-BSD® system, you will need to
  restart the ports jail to see the new user accounts. This button is not available if a Linux jail is highlighted.

- **Service Manager:** opens :ref:`Service Manager` so that you can view which services are running in the jail and configure which services should start when
  the jail is started. Note that this button is not available if a Linux jail is highlighted.

- **Launch Terminal:** opens a terminal with the root user logged into the jail. This allows you to administer the jail from the command line. This button
  will be greyed out if the highlighted jail is not running. You can start a jail by right-clicking its entry and selecting "Start this Jail" from the menu or
  by clicking "Start Jail". 

- **Export Jail:** launches a pop-up window prompting you to choose the directory in which to save a backup of the jail (and all of its software,
  configuration, and files) as a :file:`.wdn` file. Creating the :file:`.wdn` file may take some time, especially if you have installed src, ports, or
  software.

The "Snapshots" tab, shown in Figure 8.21o, is used to create and manage ZFS snapshots within the currently highlighted jail. The ZFS snapshot feature can be
used to make point in time filesystem backups of jails. A snapshot is essentially a picture of what the filesystem looked like at that point in time.
Snapshots are space efficient in that they take up zero space when created and the snapshot only grows in size as files contained within the snapshot are
modified after the snapshot was taken. In other words, ZFS manages the changes between snapshots, providing a way to return to what a file looked like at the
time a snapshot was taken. Since jails share the filesystem used by PC-BSD®, any type of jail, including a Linux jail, can take advantage of this ZFS
feature.

**Figure 8.21o: Snapshots Tab for the Highlighted Jail**

.. image:: images/warden15.png

To create a snapshot of the jail, click the "+Add" button. A pop-up menu will allow you to type in an optional comment for the snapshot. Press "OK" to create the snapshot. A
snapshot indicating the date, time, and comment will be added to the slider bar. If you create multiple snapshots at different times, use the slider bar to select a snapshot.

Once you have created a snapshot, the following actions can be used to manage the snapshot. Make sure that the desired snapshot is highlighted in the slider
bar before clicking these buttons: 

- **Restore:** returns the system to what it looked like at the time the snapshot was taken.
  **Think about what you wish to accomplish before using this option** as any changes to files that occurred after the snapshot was taken will be lost.

- **Add:** use this button to create additional snapshots.

- **Remove:** use this button to remove the highlighted snapshot.

This screen also allows you to schedule automatic snapshots. To enable this feature, check the box "Scheduled Snapshots". Use the drop-down menu to set the
frequency to daily or hourly. You can also type in or use the arrows to configure the number of days to keep each snapshot.

.. index:: jails
.. _Warden® Configuration:

Warden® Configuration
----------------------

To refresh the settings for all jails, use :menuselection:`Configure --> Refresh Jails`.

To configure Warden®, click :menuselection:`Configure --> Settings` which will open the screen shown in Figure 8.21p. 

**Figure 8.21p: Warden® Configuration**

.. image:: images/warden16.png

This screen allows you to configure the following: 

* **Jail Network Interface:** all jails created within Warden® share the same physical interface. Use the drop-down menu to select the network interface to
  be used by the jails. Note that your jails may not work if the wrong interface is configured.

* **Jail Directory:** contains all of the created jails where each jail has its own sub-directory named after its IP address. By default, it is
  :file:`/usr/jails`. If you change this directory, make sure the location has sufficient space to hold the jails.

* **Temp Directory:** used when exporting and importing jails. Make sure that the directory has sufficient space to create a tar file of the jail and its
  contents.

If you highlight a jail, its right-click menu contains the following options: 

* **Start or Stop this Jail:** allows you to start a jail (if it is currently not running) or to stop a jail (if it is currently running). You will not be
  able to access a jail that has not been started. The icon next to the jail will change to indicate the current status.

* **Toggle Autostart:** toggles a jail's Autostart between "Disabled" (does not automatically start when the PC-BSD® system is booted) and "Enabled" (will
  start the jail when the PC-BSD® system is booted). The "Info" tab will be updated to indicate the new "Start at boot" status. Note that toggling autostart
  will not affect the current running status of the jail (i.e. it does not start or stop the jail right now) as autostart is only used when the system boots.

* **Export jail to .wdn file:** allows you to save the jail (and all of its software, configuration, and files) as a :file:`.wdn` file. This allows you to
  quickly clone a pre-configured jail to a new jail on either the same or another PC-BSD® system. The exported jail will end with a :file:`.wdn` extension
  and the filename will be the IP address of the jail. When exporting a jail, a pop-up window will prompt you to choose the directory in which to store the
  backup. A progress bar will indicate that the export is in progress. Creating the :file:`.wdn` file may take some time, especially if you have installed
  src, ports, or software.

.. note:: you should close all network connections to the jail before exporting it as Warden® will need to stop the jail in order to back it up. If your jail
   is running services (e.g. a webserver), export the jail at a time that will least impact network connections to the jail.

* **Clone this Jail:** creates an instantaneous copy of the specified jail. It will prompt for a hostname for the new jail. Highlight the new clone and click
  "Jail Configuration" to set the addressing information for the new jail.

- **Delete Jail:** removes the jail and all of its contents from the PC-BSD® system. You will be prompted to confirm this action.

.. index:: jails
.. _Importing a Jail:

Importing a Jail 
----------------

The "File" menu can be used to create a new jail, import a jail, create templates, or exit Warden®. 

If you click :menuselection:`File --> Import Jail` you will be prompted to browse to the location of a previously created :file:`.wdn` file. After selecting
the file, you will then see the screen shown in Figure 8.21q. 

**Figure 8.21q: Importing a Jail** 

.. image:: images/warden17.png

Input a name for the new jail. If you are creating a new jail on the same system that still has the original jail installed, check the "IPv4 Address" box and
input an unused IP address for the new jail. Then, check the box "Hostname" and input an unused hostname for the new jail. However, if you have deleted the
original jail or need to restore that same jail on another computer (for example, there was a hardware failure on the system containing the original jail),
you can choose to leave both boxes unchecked and to reuse the same IP address and hostname. Once you press "OK", Warden® will recreate the jail with all of
the original settings. Whether or not those settings include the original IP address and hostname depends upon your selections.

.. index:: jails
.. _Template Manager:

Template Manager
----------------

The built-in template manager can be used to create and manage jail templates. Once created, templates can be used when installing a new jail. A template
specifies the version and architecture of FreeBSD to be used as the operating system running in the jail. Templates have been tested from FreeBSD versions
4.1.1 to FreeBSD-CURRENT. Until you create your own templates and specify them during jail creation, the default version and architecture of the operating
system used in the jail will be the same as that running on the PC-BSD® system.

To create a template, click :menuselection:`File --> Template Manager` to see the screen shown in Figure 8.21r. 

**Figure 8.21r: Template Manager** 

.. image:: images/warden18.png

The default icon will indicate the version of TrueOS® used by the underlying PC-BSD® system. To create a new template, click the "+" button. In the "System
Type" drop-down menu select either: 

- **TrueOS:** adds the command line versions of the PC-BSD® utilities to the FreeBSD base.

or

- **FreeBSD:** uses only the FreeBSD base without any of the PC-BSD® utilities.

Press "OK" to see the screen shown in Figure 8.21s. 

**Figure 8.21s: Select the Operating System Version** 

.. image:: images/warden19.png

If desired, change the *10.0* in this example to the release number to use. If you selected *FreeBSD* as the system type, a list of available release numbers
can be found `here <http://wiki.freebsd.org/releases/>`_. If you selected *TrueOS*, the list of available release numbers is currently limited to 9.0, 9.1,
9.2, 9.3, 10.0, and 10.1.

Press "OK". In the "System Architecture" drop-down menu, select either "amd64" (for 64-bit) or "i386" (for 32-bit). Press "OK" and input a nickname for the
template. Click "OK" and the files needed for that version will be downloaded. Once the template is created, it will appear in the Template Manager as seen in
the example in Figure 8.21t. 

**Figure 8.21t: New Template Added** 

.. image:: images/warden20.png

To delete a template, highlight it and click the "-" button. Note that Warden® will not let you delete a template if any jails exist which are using the
template.

To use the template when creating a new jail, click the "Jail Version" drop-down menu shown in Figure 8.21c and select the desired template.

.. index:: jails
.. _Using the Command Line Version of Warden®:

Using the Command Line Version of Warden® 
------------------------------------------

The Warden® GUI is based on a Bourne shell script. This script can be manually run from the command line on a PC-BSD® server or by users who prefer using
the command line. Advanced users can also refer to the command line version in their own scripts.

If you type :command:`warden` at the command line, you will receive a summary of its usage::

 warden
 Warden version 1.4
 ---------------------------------
 Available commands
 Type in help <command> for information and usage about that command
        help - This help file
         gui - Launch the GUI menu
        auto - Toggles the autostart flag for a jail
     bspkgng - BootStrap pkgng and setup TrueOS repo
     checkup - Check for updates to a jail
      chroot - Launches chroot into a jail 
      create - Creates a new jail 
     details - Display usage details about a jail
      delete - Deletes a jail
      export - Exports a jail to a .wdn file
       fstab - Start users $EDITOR on jails custom fstab
  fbsdupdate - Update the FreeBSD world inside a jail
 fbsdupgrade - Upgrade the version of FreeBSD inside a jail
         get - Gets options list for a jail
      import - Imports a jail from a .wdn file
        list - Lists the installed jails
   pkgupdate - Update packages inside a jail
        pkgs - Lists the installed packages in a jail
        pbis - Lists the installed pbis in a jail
         set - Sets options for a jail
       unset - Unsets (clears) options for a jail
       start - Start a jail
        stop - Stops a jail
        type - Set the jail type (pbibox|pluginjail|portjail|standard)
    template - Manage jail templates
        snap - Jail snapshot management 
       clone - Clone an existing jail to a new jail
    cronsnap - Schedule snapshot creation via cron

Each command has its own help text that describes its parameters and provides a usage example. For example, to receive help on how to use the
:command:`warden create` command, type::

 warden help create
 Warden version 1.4
 ---------------------------------
 Help create
 Creates a new jail, with options for system source, ports and autostarting.
 Available Flags:
 -32 Create 32bit jail on 64bit system
 --autoipv4 Use the next available IPv4 address from the pool
 --ipv4=<ip/mask> Set primary IPv4 address for jail
 --ipv6=<ip/mask> Set primary IPv6 address for jail
 --archive <tar> Use specified tar file for BSD jail creation
 --bulk <number> Create <number> of new jails, using default IP4 pool or  address pool specified with --ip4pool
 --ip4pool <address> Starting IPv4 address to use when creating jails in bulk
 --linuxjail <script> Make this a linux jail and use supplied script for  installation
 --linuxarchive <tar> Use specified tar file for Linux jail creation
 --pluginjail Make this a pluginjail
 --ports Includes the ports tree
 --portjail Make this a portjail
 --src Includes /usr/src system source
 --startauto Start this jail at system boot
 --template <string> Specify a jail template to build with
 --vanilla Don't install PC-BSD pkgng repo and utilities
 --version <string> Use this instead of /etc/version

 Usage:
 warden create <JAILNAME> <flags>

 Example:
 warden create jailbird --ipv4=192.168.0.25/24 --src --ports --startauto

You do not need superuser access to use the view commands but will for any commands that create or manage a jail. The :command:`warden` command will display
an error message if a command requires superuser access and you currently are not the superuser. On PC-BSD®, you can put :command:`pc-su` at the beginning of
the :command:`warden` command to be prompted for your password. On a FreeBSD server, you can type :command:`su` to become superuser, then repeat the
:command:`warden` command.

Before creating a jail, verify the network settings in :file:`/usr/local/etc/warden.conf`::

 #!/bin/sh
 # Configuration options for the Warden
 ######################################################################
 # Network Interface for the jails to use
 NIC:
 # Directory to use for compressing / decompressing files 
 WTMP: /usr/jails
 # Location of the jails
 JDIR: /usr/jails
 # When automatically creating jails with unspecified IPv4 addresses, use this
 # address at the starting point for new addresses
 IP4POOL: 192.168.0.220 

You can either specify the FreeBSD interface name to use in the *NIC* field or specify the IP address range starting point with the *IP4POOL* field. When
using *IP4POOL* on a network containing a DHCP server, ensure that the DHCP server has reserved the range of addresses to be used by jails in order to prevent
IP address conflicts.

To create a jail, specify a unique IP address and hostname for the jail::

 warden create jail1 --ipv4 10.0.0.1
 DEF: 10.1-RELEASE amd64
 Building new Jail... Please wait...
 <snip install messages>
 Success!
 Jail created at /usr/jails/jail1

Before you can access the jail, you will need to start it::

 warden start jail1

As the jail starts, the SSH host keys will be generated and :command:`sshd` will start. At this point, you can use the :command:`warden chroot` command to
access the jail from the host system. Alternately, to access the jail over the network using :command:`ssh`, you will need to first create a user account.

To access the jail in order to create that user::

 warden chroot jail1
 Started shell session on jail1 . Type exit when finished.

 adduser

Follow the prompts of the :command:`adduser` script in order to create a user. When you get to the following prompt, do not press enter. Instead type in
*wheel* so that the user can use the :command:`su` command to become the superuser within the jail::

 Login group is username.
 Invite username into other groups? [] **wheel**

When you are finished creating the user, you can type :command:`exit` to exit the jail. Test that :command:`ssh` works by specifying the username that you
created::

 ssh username@jail1

To create multiple jails simultaneously, use the **--bulk <number>** and **--ip4pool <starting address>** options to specify the number of jails and the
starting IP address. Alternately, instead of **--ip4pool**, use the **--autoipv4** option as it automatically assigns the next available IP address from the
pool, as defined by the *IP4POOL* option in :file:`/usr/local/etc/warden.conf`.

Table 8.21a shows the command line equivalents to the graphical options provided by the Warden® GUI. To get usage examples for each command, insert
:command:`help` into the command. For example, to get help on the **auto** command, type :command:`warden help auto`. Note that some options are only
available from the command line.

**Table 8.21a: Command Line and GUI Equivalents** 

+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **Command Line**  | **GUI**                                                              | **Description**                                                 |
+===================+======================================================================+=================================================================+
| **auto**          | right-click highlighted jail and click Autostart                     | toggles the jail's autostart between Enabled and Disabled       |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **bspkgng**       | in the GUI, this happens automatically during jail creation unless   | adds the PC-BSD utilities to an existing jail                   |
|                   | "Install PKGNG and PC-BSD utilities" is unchecked                    |                                                                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **checkup**       | in the GUI, update checks occur automatically and any un-applied     | checks for updates to either the specified jail or all jails    |
|                   | updates are shown in the "Updates" column                            |                                                                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **chroot**        | :menuselection:`Tools --> Launch Terminal`                           | opens a terminal with the root user logged into the jail        |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **create**        | "+" button or :menuselection:`File --> New Jail`                     | creates a new jail with specified attributes                    |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **details**       | Info tab                                                             | provides an overview of specified jail's configuration          |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **delete**        | "-" button or right-click jail --> Delete Jail                       | deletes the specified jail                                      |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **export**        | right-click --> Export jail to :file:`.wdn` file                     | saves the specified jail and all of its software, configuration,|
|                   |                                                                      | and files as a :file:`.wdn` file                                |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **fbsdupdate**    | :menuselection:`Tools --> Check for Updates`                         | upgrades FreeBSD world with security fixes as well as any       |
|                   |                                                                      | package updates                                                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **fbsdupgrade**   | :menuselection:`Tools --> Check for Updates`                         | upgrades FreeBSD to new version                                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **fstab**         |                                                                      | opens the jail's :file:`/etc/fstab` in an editor                |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **get**           | configure (wrench) icon for highlighted jail                         | lists the various IP addresses used by the jail                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **import**        | :menuselection:`File --> Import Jail`                                | import a previously created :file:`.wdn` file                   |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **list**          | "Installed Jails" section of GUI                                     | list all jails                                                  |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **pkgupdate**     | :menuselection:`Tools --> Check for Updates`                         | update all packages in specified jail                           |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **pkgs**          | :menuselection:`Tools --> AppCafe`                                   | lists packages installed into specified jail                    |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **pbis**          |                                                                      | list PBIs installed into specified jail                         |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **set**           | right-click jail                                                     | used to set options, addresses, aliases, and permissions in     |
|                   |                                                                      | specified jail                                                  |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **unset**         | right-click jail                                                     | used to unset options, addresses, aliases, and permissions in   |
|                   |                                                                      | specified jail                                                  |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **start**         | right-click jail --> Start this Jail                                 | starts the specified jail                                       |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **stop**          | right-click jail --> Stop this Jail                                  | stops the specified jail                                        |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **type**          | "Jail Type" during jail creation                                     | types differ as choices are *pbibox*, *portjail*, *pluginjail*, |
|                   |                                                                      | or *standard*; to create a Linux jail, instead use the          |
|                   |                                                                      | *linuxjail* option with the **create** command                  |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **template**      | :menuselection:`File --> Template Manager`                           | used to create, delete, or list templates                       |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **snap**          | Snapshots                                                            | snapshot management for specified jail                          |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **clone**         | right-click --> Clone this Jail                                      | clones an existing jail                                         |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
| **cronsnap**      | :menuselection:`Snapshots --> Scheduled Snapshots`                   | schedules ZFS snapshot creation                                 |
+-------------------+----------------------------------------------------------------------+-----------------------------------------------------------------+
