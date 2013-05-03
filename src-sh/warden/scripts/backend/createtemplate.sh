#!/bin/sh
# Script to create a new jail template
#####################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

### Download the template files
download_template_files() {

  # Create the download directory
  if [ -d "${JDIR}/.download" ] ; then rm -rf ${JDIR}/.download; fi
  mkdir ${JDIR}/.download

  if [ ! -d "${JDIR}" ] ; then mkdir -p "${JDIR}" ; fi
  cd ${JDIR}

  echo "Fetching jail environment. This may take a while..."
  if [ -n "$TRUEOSVER" ] ; then
     for f in $DFILES
     do
       get_file_from_mirrors "/${TRUEOSVER}/${FBSDARCH}/dist/$f" "${JDIR}/.download/$f"
       if [ $? -ne 0 ] ; then
         exit_err "Failed downloading: /${TRUEOS}/${FBSDARCH}/dist/${f}"
       fi
     done
  else
     for f in $DFILES
     do
       fetch -o "${JDIR}/.download/$f" "ftp://ftp.freebsd.org/pub/FreeBSD/releases/${FBSDARCH}/${FBSDVER}/$f"
       if [ $? -ne 0 ] ; then
	 echo "Trying ftp-archive..."
         fetch -o "${JDIR}/.download/$f" "http://ftp-archive.freebsd.org/pub/FreeBSD-Archive/old-releases/${FBSDARCH}/${FBSDVER}/$f"
         if [ $? -ne 0 ] ; then
           exit_err "Failed downloading: FreeBSD ${FBSDVER}"
	 fi
       fi
     done
  fi

}

create_template()
{
  # Creating ZFS dataset?
  isDirZFS "${JDIR}"
  if [ $? -eq 0 ] ; then
    local zfsp=`getZFSRelativePath "${TDIR}"`

    # Use ZFS base for cloning
    tank=`getZFSTank "${JDIR}"`
    isDirZFS "${TDIR}" "1"
    if [ $? -ne 0 ] ; then
       echo "Creating ZFS ${TDIR} dataset..."
       zfs create -o mountpoint=/${zfsp} -p ${tank}${zfsp}
       if [ $? -ne 0 ] ; then exit_err "Failed creating ZFS base dataset"; fi
    fi

    # Using a supplied tar file?
    if [ -n "$FBSDTAR" ] ; then
        tar xvpf $FBSDTAR -C ${TDIR} 2>/dev/null
        if [ $? -ne 0 ] ; then exit_err "Failed extracting: $FBSDTAR"; fi
    else
      # Extract the dist files
      for f in $DFILES
      do
        tar xvpf ${JDIR}/.download/$f -C ${TDIR} 2>/dev/null
        if [ $? -ne 0 ] ; then exit_err "Failed extracting ZFS template environment"; fi
        rm ${JDIR}/.download/${f}
      done
    fi

    zfs snapshot ${tank}${zfsp}@clean
    if [ $? -ne 0 ] ; then exit_err "Failed creating clean ZFS base snapshot"; fi
  else
    # Sigh, still on UFS??
    if [ -d "${JDIR}/.templatedir" ]; then
       rm -rf ${JDIR}/.templatedir
    fi
    mkdir ${JDIR}/.templatedir

    if [ -n "$FBSDTAR" ] ; then
      # User-supplied tar file 
      cp $FBSDTAR ${TDIR}
    else
      # Extract the dist files
      for f in $DFILES
      do
        tar xvpf ${JDIR}/.download/$f -C ${JDIR}/.templatedir 2>/dev/null
        if [ $? -ne 0 ] ; then 
           rm -rf ${JDIR}/.templatedir
           exit_err "Failed extracting ZFS template environment"
        fi
        rm ${JDIR}/.download/${f}
      done

      echo "Creating template archive..."
      tar cvjf ${TDIR} -C ${JDIR}/.templatedir 2>/dev/null
      rm -rf ${JDIR}/.templatedir
    fi
  fi

  echo "Created jail template: $TNICK"
  exit 0
};


# Read our flags
while [ $# -gt 0 ]; do
   case $1 in
    -fbsd) shift
           if [ -z "$1" ] ; then exit_err "No FreeBSD version specified"; fi
           FBSDVER="${1}"
           ;;
  -trueos) shift
           if [ -z "$1" ] ; then exit_err "No TrueOS version specified"; fi
           TRUEOSVER="${1}"
           ;;
    -arch) shift
           if [ -z "$1" ] ; then exit_err "No FreeBSD architecture specified"; fi
           FBSDARCH="${1}"
           ;;
    -tar) shift
           if [ -z "$1" ] ; then exit_err "No tar file specified"; fi
           if [ ! -e "$1" ] ; then exit_err "Could not find tar file: $1"; fi
           FBSDTAR="${1}"
           ;;
    -nick) shift
           if [ -z "$1" ] ; then exit_err "No nickname specified"; fi
           TNICK="${1}"
	   ;;
	*) exit_err "Invalid option: $1" ;;
   esac
   shift
done

if [ -z "$TNICK" ] ; then exit_err "No nickname specified, use -nick <nickname>"; fi
if [ -z "$FBSDTAR" ] ; then 
  if [ -z "$FBSDVER" -a -z "${TRUEOSVER}" ] ; then exit_err "Need either -fbsd or -trueos specified!"; fi
  case $FBSDARCH in
   amd64) if [ "`uname -m`" != "amd64" ] ; then exit_err "Can only use amd64 on amd64 systems"; fi ;;
    i386) ;;
       *) exit_err "Arch needs to be amd64 or i386" ;;
  esac
fi

# Set the template directory
TDIR="${JDIR}/.warden-template-$TNICK"

# Set the name based upon if using ZFS or UFS
isDirZFS "${JDIR}"
if [ $? -eq 0 ] ; then
  TDIR="${TDIR}"
else
  TDIR="${TDIR}.tbz"
fi

# Make sure this template is available
if [ -e "${TDIR}" ] ; then 
  exit_err "A template with this nickname already exists!"
fi

# Set the files we will be downloading
DFILES="base.txz doc.txz games.txz"
if [ "$FBSDARCH" = "amd64" ] ; then
  DFILES="$DFILES lib32.txz"
fi

# If not using a tarball, lets download our files
if [ -z "$FBSDTAR" ] ; then
  download_template_files
fi

# Create the template now
create_template

