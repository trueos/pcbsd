#!/bin/sh
# Script to perform some first boot setup

# TrueOS Scripts Dir
sDir="/usr/local/share/pcbsd/scripts"

# Set default mixer volume
mixer vol 100:100 >/dev/null 2>/dev/null
mixer pcm 100:100 >/dev/null 2>/dev/null
