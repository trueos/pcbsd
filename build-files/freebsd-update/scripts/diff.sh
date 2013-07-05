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

# $FreeBSD: user/cperciva/freebsd-update-build/scripts/diff.sh 247826 2013-03-05 00:52:14Z cperciva $

# Perform a build, including any patches in the patches/${RELEASE}
# directory, and identify the updates which need to be propagated to
# the FreeBSD Update clients.

. `dirname $0`/build.subr
readconfig $1 $2

# Make sure that we've run init.sh first
checkinit

# Sanity-check the specified patch number; or construct it by adding
# one to the previous patch number.
patchnumber $3

# Perform the first build
extractworld world0
patchworld world0
buildworld world0
addextradocs world0
indexfiles world0

# Perform the second build
extractworld world1
patchworld world1
futurebuildworld world1
addextradocs world1
indexfiles world1

# Compare world0 and world1 to find stamps
findstamps world0 world1

# Now that we have found the stamps, we don't need the future world.
removeworld world1

# Edit world0-index to
# * Revert binaries to the versions in the most recent shipped build if
#   they have not changed aside from buildstamps, and
# * Revert binaries to the versions shipped in the release if they have
#   not changed since the first build.
unstamp world0

# Move everything into the staging area
stageworld world0

# Warn the user about which timestamps have been found
printstamps

# Warn about new updates
printupdates

# Tell the user that we're done and what he should do next
cat <<-EOF

	FreeBSD/${TARGET} ${REL} update build complete.  Please review
	the list of build stamps printed above and the list of updated
	files to confirm that they look sensible, then run
	# sh -e approve.sh ${TARGET} ${REL}
	to sign the build.
EOF
