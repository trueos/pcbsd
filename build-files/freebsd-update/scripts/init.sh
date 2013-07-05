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

# $FreeBSD: user/cperciva/freebsd-update-build/scripts/init.sh 247826 2013-03-05 00:52:14Z cperciva $

# Perform an "initialization" build for a release.  This downloads the
# release ISO image and builds everything locally in order to find any
# build stamps.  As the name suggests, this script is the first thing which
# should be run for each release, and should only be run once.

. `dirname $0`/build.subr
readconfig $1 $2

# Make sure that necessary binaries have been built
checkbins

# Create directories, (empty) databases, and some incidental files
# in the publishing directory.
makedirs
makedbs
makepub

# Fetch the ISO and extract the important files.
fetchiso
extractiso

# Perform a build.
extractworld world0
rpatchworld world0
buildworld world0

# Compare the built and released worlds to identify
# which docs are built from the doc tree; add them
# to the built files.
findextradocs world0
addextradocs world0

# Fully index the released and built files.
indexfiles release
indexfiles world0

# Check for discrepancies
diffwarn world0

# Perform build 400 days into the future
extractworld world1
rpatchworld world1
futurebuildworld world1
addextradocs world1

# Index the future world
indexfiles world1

# Compare world0 and world1 to find stamps
findstamps world0 world1

# Compare world0 and the release to find differences in non-stamped files.
findnonstamps world0

# Now that we have found the stamps, we don't need the worlds we built.
removeworld world0
removeworld world1

# Move everything from the release into the staging area
stageworld release

# Warn the user about which timestamps have been found
printstamps

# Tell the user that we're done and what he should do next
cat <<-EOF

	FreeBSD/${TARGET} ${REL} initialization build complete.  Please
	review the list of build stamps printed above to confirm that
	they look sensible, then run
	# sh -e approve.sh ${TARGET} ${REL}
	to sign the release.
EOF
