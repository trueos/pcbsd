#!/bin/sh
#
# Copyright 2014 Kris Moore (iXsystems)
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted providing that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh

# Define some defaults
MPDIR="/root/pkgset"

# Function to list available metapkgs
list_metapkgs() {
	if [ ! -d "${MPDIR}" ] ; then
		if [ -d "./pkgset" ] ; then
			MPDIR="`realpath ./pkgset`"
		else
			exit_err "No available meta-pkg dir"
		fi
	fi

	# Start listing each meta-pkg
	for i in `find ${MPDIR}/* -type d`
	do
		# Don't list network-only packages unless we are on network install media
		if [ ! -e "/pcbsd-media-network" -a -e "${i}/network-only" ] ; then
			continue
		fi
		if [ -e "${i}/pkg-desc" ] ; then
			desk="NO"
			echo "$i" | grep -q "Desktop"
			if [ $? -eq 0 ] ; then
				desk="YES"
			fi

			echo "Meta Package: `basename ${i}`"
			echo "-------------------------------------"
			echo "Description: `cat ${i}/pkg-desc`"
			if [ -e "${i}/pkg-icon.png" ] ; then
				echo "Icon: ${i}/pkg-icon.png"
			fi
			echo "Desktop: $desk"
			if [ -e "${i}/pkg-list" ] ; then
				echo "Parent: `basename $(dirname $i)`"
				echo " "
				echo "Required Packages:"
				cat ${i}/pkg-list
			else
				echo " "
				echo "Category Entry"
			fi
			echo " "
		fi
	done

}


# List available pkgs
list_metapkgs

exit 0

