#!/bin/sh
# Configuration options for src-sh applications

# Set the PC-BSD version number
# When you update this tag, be sure to update ../src-qt4/config.h as well
export PCBSDVERSION="master"

# When rolling ISOs, what package set to use on fresh installs?
# {PRODUCTION|EDGE|ENTERPRISE}
export INSTALLPACKAGESET="EDGE"
