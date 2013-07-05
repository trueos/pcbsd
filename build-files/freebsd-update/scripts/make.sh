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

# $FreeBSD: user/cperciva/freebsd-update-build/scripts/make.sh 247826 2013-03-05 00:52:14Z cperciva $

# Build binaries used by FreeBSD Update builds and generate an RSA signing
# key.  The SHA256 hash of the public key is printed and must be distributed
# to clients in a secure manner.

# NOTE: This script must be run before anything else (even init.sh), and
# MUST ONLY BE RUN ONCE.  Running this script again will overwrite your
# signing key.

. `dirname $0`/build.subr
readglobalconfig
makeglobaldirs

# Make programs for finding and removing build stamps.
makebin

# Generate the RSA key to be used for signing everything.
makekey

# Encrypt and save the RSA key
encryptkey

# Unmount the unencrypted key
umountkeydir
