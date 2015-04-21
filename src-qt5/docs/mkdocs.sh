#!/bin/sh


# Create the HTML
gmake html
if [ $? -ne 0 ] ; then
  exit 1
fi

# Create the PDF's
#yes '' | gmake latexpdf
#yes '' | gmake latexpdf
#if [ $? -ne 0 ] ; then
#  exit 1
#fi

# Check if we need to install docs somewhere
if [ -z "$1" ] ; then exit 0; fi

if [ -d "${1}/share/pcbsd/doc/html" ] ; then
  rm -rf ${1}/share/pcbsd/doc/html
fi
if [ ! -d "${1}/share/pcbsd/doc" ] ; then
  mkdir -p ${1}/share/pcbsd/doc
fi

# Copy over the docs
cp -r _build/html ${1}/share/pcbsd/doc

exit 0
