#!/bin/sh
# Set nzbget to 755, it was shipped with 655 by default which no worky when installing as user
chmod 755 ${PBI_PROGDIRPATH}/local/bin/nzbget
