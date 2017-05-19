# pcbsd

The official PC-BSD git repository

## Source Map

`build-files/`

Contains iso image configuration files, package settings, dummy port settings, and build settings.

`overlays/`
 
Files included on our install images, scripts, and role settings for the installer.

`src-qt5/`
 
C++ Code for pcbsd-utils-qt5 package, which builds into all the various PC-BSD related GUI utilities.

Note: Qt5 build tools are located in `/usr/local/lib/qt5/bin/` rather than `/usr/local/bin/`

*Requires Qt5 to build / run*

To create the Makefile in src-qt5, make sure `devel/qt5` is installed on your system and then run:

```
% cd src-qt5 && /usr/local/lib/qt5/bin/qmake
```

`pbi-modules/`

Contains PC-BSD's PBI .conf files.

`retired/`

Any PC-BSD utilities or files that are no longer in use.

`src-sh/`

Shell code which builds into pcbsd-utils package. Scripts, backends and various CLI
related utilities for TrueOS & PC-BSD. 

`src-webui/`

All the code for the AppCafe WEB interface.

#### Contacting Us

###### IRC (FreeNode)

`#pcbsd-dev` (Developer questions / talk)

`#pcbsd` (General user information)

###### Mailing Lists

http://lists.pcbsd.org

###### Bug Tracker

http://bugs.pcbsd.org

**General Project Information: Table of Contents**

- [General TrueOS Information](#gentrosinfo)
	- [TrueOS Project Documentation](#docs)
		- [TrueOS Handbook](#trueosdoc)
		- [Lumina Handbook](#luminadoc)
		- [SysAdm Handbooks](#sysadmdoc)
	- [Filing Issues or Feature Requests](#fileissues)
	- [Community Channels](#community)
		- [Discourse](#discourse)
		- [Gitter](#gitter)
		- [IRC](#irc)
		- [Subreddit](#subreddit)
	- [Social Media](#socialmedia)

<!-- END GENERAL INFO TOC -->

# General TrueOS Information <a name="gentrosinfo"></a>

This section describes where you can find more information about TrueOS and its related projects, file new issues on GitHub, and converse with other users or contributors to the project.

## TrueOS Project Documentation <a name="docs"></a>

A number of [Sphinx](http://www.sphinx-doc.org/en/stable/) generated reStructuredText handbooks are available to introduce you to the TrueOS, Lumina, and SysAdm projects. These handbooks are open source, and users are always encouraged to open GitHub issues or fix any errors they find in the documentation.

### TrueOS Handbook <a name="trueosdoc"></a>

The [TrueOS User Guide](https://www.trueos.org/handbook/trueos.html) is a comprehensive guide to install TrueOS, along with post-installation configuration help, recommendations for useful utilities and applications, and a help and support section containing solutions for common issues and links to community and development chat channels for uncommon issues. There is also a chapter describing the experimental TrueOS Pico project and links to the Lumina and SysAdm documentation. All TrueOS documentation is hosted on the [TrueOS website](https://www.trueos.org).

### Lumina Handbook <a name="luminadoc"></a>

The Lumina Desktop Environment has its own [handbook](https://lumina-desktop.org/handbook/), hosted on the [Lumina Website](https://lumina-desktop.org). This handbook contains brief installation instructions. However, due to the highly customizable nature of Lumina, the focus of the handbook lies mainly in documenting all user configurable settings. Each option is typically described in detail, with both text and screenshots. Finally, the suite of unique Qt5 utilities included with Lumina are also documented.

TrueOS users are encouraged to review the Lumina documentation, as the Lumina Desktop Environment is installed by default with TrueOS.

### SysAdm Handbooks <a name="sysadmdoc"></a>

Due to complexity of this project, SysAdm documentation is split into three different guides:

1. **API Reference Guide** (https://api.sysadm.us/getstarted.html)

The Application Programming Interface (API) Reference Guide is a comprehensive library of all API calls and WebSocket requests for SysAdm. In addition to documenting all SysAdm subsystems and classes, the guide provides detailed examples of requests and responses, authentication, and SSL certificate management. This guide is constantly updated, ensuring it provides accurate information at all times.

2. **Client Handbook** (https://sysadm.us/handbook/client/)

The SysAdm Client handbook documents all aspects of the SysAdm client, as well as describing of the PC-BSD system utilities is replaces. Detailed descriptions of utilities such as Appcafe, Life Preserver, and the Boot Environment Manager are contained here, as well as a general guide to using these utilities. TrueOS users are encouraged to reference this guide, as the SysAdm client is included with TrueOS.

3. **Server Handbook** (https://sysadm.us/handbook/server/introduction.html)

The Server handbook is a basic installation guide, walking new users through the process of initializing SysAdm with a bridge and server connection.

## Filing Issues or Feature Requests <a name="fileissues"></a>

Due to the number of repositories under the TrueOS "umbrella", the TrueOS Project consolidates its issue trackers into a few repositories:

* [trueos-core](https://github.com/trueos/trueos-core) : Used for general TrueOS issues, Pico issues, and feature  requests.
* [lumina](https://github.com/trueos/lumina) : Issues related to using the Lumina Desktop Environment.
* (Coming Soon) [sysadm](https://github.com/trueos/sysadm) : Issues with using the SysAdm client or server.
* [trueos-docs](https://github.com/trueos/trueos-docs) : Issues related to the TrueOS Handbook.
* [lumina-docs](https://github.com/trueos/lumina-docs) : Issues related to the Lumina Handbook.
* [sysadm-docs](https://github.com/trueos/sysadm-docs) : Issues related to the SysAdm API Guide, Client, and Server Handbooks.
* [trueos-website](https://github.com/trueos/trueos-website) : Issues involving any of the TrueOS Project websites: 
  - https://www.lumina-desktop.org
  - https://www.trueos.org
  - https://www.sysadm.us

The TrueOS handbook has detailed instructions to help you report a bug (https://www.trueos.org/handbook/helpsupport.html#report-a-bug). It is recommended to refer to these instructions when creating new GitHub issues. Better bug reports usually result in faster fixes!

To request a feature, open a new issue in one of the related GitHub issue repositories and begin the title with *Feature Request:*.

## Community Channels <a name="community"></a>

The TrueOS community has a wide variety of chat channels and forum options available for users to interact with not only each other, but contributors to the project and the core development team too.

### Discourse <a name="discourse"></a>

TrueOS  has a [Discourse channel](https://discourse.trueos.org/) managed concurrently with the TrueOS Subreddit. New users need to sign up with Discourse in order to create posts, but it is possible to view posts without an account.

### Gitter <a name="gitter"></a>

The TrueOS Project uses Gitter to provide real-time chat and collaboration with TrueOS users and developers. Gitter does not require an application to use, but does require a login using either an existing GitHub or Twitter account.

To access the TrueOS Gitter community, point a web browser to https://gitter.im/trueos.

Gitter also maintains a full archive of the chat history. This means lengthy conversations about hardware issues or workarounds are always available for reference. To access the Gitter archive, navigate to the desired TrueOS room’s archive. For example, here is the address of the TrueOS Lobby archive: https://gitter.im/trueos/Lobby/archives.

### IRC <a name="irc"></a>

Like many open source projects, TrueOS has an Internet Relay Chat (IRC) channel so users can chat and get help in real time. To get connected, use this information in your IRC client:

* Server name: irc.freenode.net
* Channel name: #trueos (note the # is required)

### Subreddit <a name="subreddit"></a>

The TrueOS Project also has a [Subreddit](https://www.reddit.com/r/TrueOS/) for users who prefer to use Reddit to ask questions and to search for or post how-tos. A Reddit account is not required in order to read the Subreddit, but it is necessary to create a login account to submit or comment on posts.

## Social Media <a name="socialmedia"></a>

The TrueOS Project also maintains a number of social media accounts you can watch:

* Facebook: https://www.facebook.com/groups/4210443834/
* Linkedin: http://www.linkedin.com/groups?gid=1942544
* TrueOS Blog: https://www.trueos.org/blog/
* Twitter: https://twitter.com/TrueOS_Project/

