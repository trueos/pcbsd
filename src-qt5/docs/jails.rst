.. index:: jails
.. _Jails:

Jails
*****

Jails are the FreeBSD version of things like docker.  They were designed with security first and then built from there.  They run on bare metal and do not require emulation or a hypervisor so you get very good performance from them.  The traditional way to setup jails was with the ezjail port or manually.  PC-BSD introduced Warden which was a GUI way of implementing them.  The latest release uses iocage.

.. index:: Getting Started
.. _Getting STarted:

Getting Started
===============

To get started you need to first make sure iocage is installed with :command:`pkg install iocage`.  Once installed you need to grab the package sets for the release you plan on using.  When you run :command:`iocage fetch` you will be given a few options.  Right now I have 10.2-RELEASE and 9.2-RELEASE.'  You can then type which one you'd like and it will procede to get all the package sets to create your jails from.   You probably also want a base jail to build your other jails from you can do this with :command:`iocage -b`. 

.. Index:: Creating a New Jail the Easy Way
.. _Creating a New Jail the Easy Way:

Creating a New Jail the Easy Way
================================

You can create a new jail by simply typing :command:`iocage create` though you probably want to name your jail and give it an IP.  To do that you can run :command: `iocage create -c tag=<name of jail> ip="<networkcard>:<ip>/<subnet>"`.  For the IP/Networking part you will want to use an alias to your network card most likely.  You can do this by doing :command: `echo 'ifconfig_<networkadapter>_alias0="inet <ip>/<subnet>"' >> /etc/rc.conf`.  To access your jail you can do :command:`iocage console <jailname>`.  From there you can start your jailed services or do whatever it is you'd like separated out.  
