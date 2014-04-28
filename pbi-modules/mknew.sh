#!/bin/sh

echo "$1" | grep -q '/'
if [ $? -ne 0 ] ; then
  echo "Usage: ./mknew.sh category/port"
  exit 1
fi

name=`basename $1`
cat=`dirname $1`
if [ ! -d "$cat" ] ; then
   echo "Error: Category $cat does not exist! Make the directory first."
   exit 1
fi

catName=`grep "::::$cat::::" PBI-categories | awk -F:::: '{print $1}' | sed 's|Cat=||g'`
if [ -z "$catName" ] ; then
   echo "Error: Category $cat does not exist in PBI-categories"
   exit 1
fi

# Make sure this doesn't already exist
if [ -d "$1" ] ; then
   echo "Error: This module already exists! $1"
   exit 1
fi

name=`pkg rquery '%n' $1`
if [ -n "$name" ] ;then
  web=`pkg rquery '%w' $1`
  auth="The $pName Team"
  lic=`pkg rquery '%L' $1 | head -n 1`
  type="Text"
   pkg rquery '%do' $1 | grep -q "x11"
   if [ $? -eq 0 ] ; then
      type="Graphical"
   fi
else

  echo "Could not find package $1 in database.. Manual mode enabled"
  echo " "

  echo "Enter the program name"
  echo -e ">\c"
  read name

  echo "Enter the program website"
  echo -e ">\c"
  read web

  echo "Enter the program author"
  echo -e ">\c"
  read auth

  echo "Enter the program license"
  echo -e ">\c"
  read lic

  echo "Enter the program search keywords - using , between"
  echo -e ">\c"
  read tags

  echo "Enter program type (Graphical,Text,Server)"
  echo -e ">\c"
  read type

fi

mkdir ${1}

cat << EOF >${1}/pbi.conf
#!/bin/sh
# PBING Module Config

# -- Program Base Information --
PBI_ORIGIN="$1"
PBI_PROGNAME="$name"
PBI_PROGWEB="$web"
PBI_PROGAUTHOR="$auth"

# -- Additional repo information (optional) --
PBI_LICENSE="$lic"
PBI_TAGS="$tags"
PBI_PROGTYPE="$type"
PBI_CATEGORY="$catName"

# -- Additional package to install along with ORIGIN
PBI_OTHERPKGS=""

# -- Optional related packages to show user
PBI_PLUGINS=""

# -- Space delimited list of URLs to screenshots
PBI_SCREENSHOTS=""

# -- Other PBIs which are similar to this PBI
PBI_RELATED=""

export PBI_ORIGIN PBI_PROGNAME PBI_PROGWEB PBI_PROGAUTHOR
export PBI_LICENSE PBI_TAGS PBI_PROGTYPE PBI_CATEGORY
export PBI_OTHERPKGS PBI_PLUGINS
export PBI_SCREENSHOTS PBI_RELATED
EOF

echo "Created $1/pbi.conf"
echo "Don't forget to add ${1}/icon.png (64x64 minimum)"
