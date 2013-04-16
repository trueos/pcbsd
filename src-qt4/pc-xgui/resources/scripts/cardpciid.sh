#!/bin/sh

# Get the PCI ID of the main GPU
INMON="0"

VGAID="`pciconf -l | grep vgapci0 | sed 's|^.*chip=||' | cut -d " " -f 1 | cut -c 1-6`"
echo $VGAID
