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

# $FreeBSD: user/cperciva/freebsd-update-build/scripts/approve.sh 247826 2013-03-05 00:52:14Z cperciva $

# Mark a build as "approved": Sign it and copy it to the upload staging
# area.  This should be done after carefully examining the output of the
# recently complete "init" or "diff" run.  Note that once a build has
# been approved, it is rather difficult to "unapprove" it, even if nothing
# has yet been uploaded.

. `dirname $0`/build.subr
readconfig $1 $2

# Make sure an unencrypted key exists
checkkey

# Approve the build.
approve

# Remove the world; we don't need it any more
removeworld newworld

# Remind the user to umount the key if they're finished
cat <<-EOF

	The FreeBSD/${TARGET} ${RELP} update build has been signed and is
	ready to be uploaded.  Remember to run
	# sh -e umountkey.sh
	to unmount the decrypted key once you have finished signing all
	the new builds.
EOF
