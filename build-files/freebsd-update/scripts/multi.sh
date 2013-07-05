#!/bin/sh -e

#-
# Copyright 2006 Colin Percival
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

# $FreeBSD: user/cperciva/freebsd-update-build/scripts/multi.sh 247826 2013-03-05 00:52:14Z cperciva $

# Script for doing the same operation (init / approve / diff / upload)
# to multiple releases.  This helps avoid typos at the command line
# when the same security patch needs to be applied to half a dozen
# releases...

. `dirname $0`/build.subr
readglobalconfig
makeglobaldirs

# Make sure we have the right number of arguments
if [ $# -ne 3 ]; then
	echo "usage: $0 script \"i386 ...\" \"5.5-RELEASE ...\""
	exit 1
fi

SCRIPT=$1; shift
TARGETS=$1; shift
RELEASES=$1; shift

SCRIPTPATH="${SCRIPTDIR}/${SCRIPT}.sh"

# Make sure the script is something sensible
if ! [ -x ${SCRIPTPATH} ]; then
	echo "${SCRIPTPATH} does not exist or is not executable"
	exit 1
fi

# Do the work, and log output into ${BASEDIR}/logs.  While the
# output produced from these scripts (e.g., filestamps) can be
# obtained from ${WORKDIR}, it's best if the information doesn't
# get lost to scrolling windows in the first place.
for TARGET in ${TARGETS}; do
	for RELEASE in ${RELEASES}; do
		${SCRIPTPATH} ${TARGET} ${RELEASE} |
		    tee ${BASEDIR}/logs/${SCRIPT}-${TARGET}-${RELEASE}
	done
done
