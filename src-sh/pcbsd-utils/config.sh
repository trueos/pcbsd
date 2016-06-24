#!/bin/sh
# Configuration options for src-sh applications

# Set the TrueOS version number
# When you update this tag, be sure to update ../src-qt4/config.h as well
export PCBSDVERSION="master"

# When rolling ISOs, what package set to use on initial install?
# {PRODUCTION|EDGE|ENTERPRISE}
export INSTALLPACKAGESET="EDGE"
