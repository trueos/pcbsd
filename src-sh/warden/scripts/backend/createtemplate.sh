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

     # If we are doing a first-time jail "prime"
     if [ "$WARDENPRIME" = "TRUE" ] ; then
        mv /usr/local/tmp/warden-dist/*.txz ${JDIR}/.download/
        rmdir /usr/local/tmp/warden-dist
     else
       for f in $DFILES
       do
         get_file_from_mirrors "/${TRUEOSVER}/${FBSDARCH}/dist/$f" "${JDIR}/.download/$f" "iso"
         if [ $? -ne 0 ] ; then
           exit_err "Failed downloading: /${TRUEOS}/${FBSDARCH}/dist/${f}"
         fi
       done
     fi
  else
     
     # Start looking for current versions of FreeBSD
     found=0
     for f in $DFILES
     do
       fetch -o "${JDIR}/.download/$f" "ftp://ftp.freebsd.org/pub/FreeBSD/releases/${FBSDARCH}/${FBSDARCH}/${FBSDVER}/$f"
       if [ $? -ne 0 ] ; then
	 echo "Trying ftp-archive..."
         fetch -o "${JDIR}/.download/$f" "http://ftp-archive.freebsd.org/pub/FreeBSD-Archive/old-releases/${FBSDARCH}/${FBSDARCH}/${FBSDVER}/$f"
	 if [ $? -ne 0 ] ; then found=1 ; break; fi
       fi
     done

     if [ $found -eq 0 ] ; then return ; fi

     found=0
     # Check if we are on old versions of FreeBSD
     # Get the .inf list file
     echo "Looking for old versions of FreeBSD"
     fetch -o "${JDIR}/.download/${oldStr}.inf" "ftp://ftp.freebsd.org/pub/FreeBSD/releases/${FBSDARCH}/${FBSDVER}/$oldStr/${oldStr}.inf"
     if [ $? -ne 0 ] ; then
       # Check the ftp-archive
       echo "Looking for older versions of FreeBSD"
       fetch -o "${JDIR}/.download/${oldStr}.inf" "http://ftp-archive.freebsd.org/pub/FreeBSD-Archive/old-releases/${FBSDARCH}/${FBSDVER}/${oldStr}/${oldStr}.inf"
       if [ $? -ne 0 ] ; then
         exit_err "Failed downloading: FreeBSD ${FBSDVER} - ${oldStr}.inf"
       fi
       fDir="http://ftp-archive.freebsd.org/pub/FreeBSD-Archive/old-releases/${FBSDARCH}/${FBSDVER}/${oldStr}"
     else
       fDir="ftp://ftp.freebsd.org/pub/FreeBSD/releases/${FBSDARCH}/${FBSDVER}/$oldStr"
     fi

     # Now read in the list of files to fetch
     while read line
     do
        echo "$line" | grep -q '^cksum'
        if [ $? -ne 0 ] ; then continue ; fi
        fName=`echo $line | cut -d " " -f 1 | sed "s|cksum|$oldStr|g"`
        fetch -o "${JDIR}/.download/$fName" "${fDir}/${fName}"
        if [ $? -ne 0 ] ; then
          exit_err "Failed downloading: FreeBSD ${FBSDVER}"
        fi
     done < ${JDIR}/.download/${oldStr}.inf
  fi

}

create_template()
{
  # Get the dataset of the jails mountpoint
  rDataSet=`mount | grep "on ${JDIR} " | awk '{print $1}'`
  tSubDir=`basename $TDIR`
  nDataSet="${rDataSet}/${tSubDir}"

  echo "Creating ZFS ${nDataSet} dataset..."
  zfs create -p ${nDataSet}
  if [ $? -ne 0 ] ; then exit_err "Failed creating ZFS base dataset"; fi

  # Using a supplied tar file?
  if [ -n "$FBSDTAR" ] ; then
    tar xvpf $FBSDTAR -C ${TDIR} 2>/dev/null
    if [ $? -ne 0 ] ; then exit_err "Failed extracting: $FBSDTAR"; fi
  elif [ "$oldFBSD" = "YES" ] ; then
    cd ${JDIR}/.download/
    cat ${oldStr}.?? | tar --unlink -xpzf - -C ${TDIR} 2>/dev/null
    cd ${JDIR}
  else
    # Extract the dist files
    for f in $DFILES
    do
      tar xvpf ${JDIR}/.download/$f -C ${TDIR} 2>/dev/null
      if [ $? -ne 0 ] ; then exit_err "Failed extracting ZFS template environment"; fi
      rm ${JDIR}/.download/${f}
    done
  fi

  # Creating a plugin jail?
  if [ "$TPLUGJAIL" = "YES" ] ; then
    cp /etc/resolv.conf ${TDIR}/etc/resolv.conf
    bootstrap_pkgng "${TDIR}" "pluginjail"
  fi

  zfs snapshot ${nDataSet}@clean
  if [ $? -ne 0 ] ; then exit_err "Failed creating clean ZFS base snapshot"; fi

  rm -rf ${JDIR}/.download
  echo "Created jail template: $TNICK"
  exit 0
};


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

# Make sure this template is available
if [ -e "${TDIR}" ] ; then 
  exit_err "A template with this nickname already exists!"
fi

# Set the files we will be downloading
DFILES="base.txz doc.txz games.txz"
if [ "$FBSDARCH" = "amd64" ] ; then
  DFILES="$DFILES lib32.txz"
fi

# Check if we are on REAL old versions of FreeBSD
if [ -n "$FBSDVER" ] ; then
  mV=`echo $FBSDVER | cut -d '-' -f 1 | cut -d '.' -f 1`
  if [ $mV -lt 9 ] ; then 
     oldFBSD="YES"
     oldStr="base"
  fi
  if [ $mV -lt 5 ] ; then 
     # VERY VERY old!
     oldFBSD="YES"
     oldStr="bin"
  fi
fi


# If not using a tarball, lets download our files
if [ -z "$FBSDTAR" ] ; then
  download_template_files
fi

# Create the template now
create_template

exit 0
