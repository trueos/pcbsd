#ifndef REPORT_PLACES_H
#define REPORT_PLACES_H

#include <QPicture>

#define TRAC_PICTURE ":/images/trac_icon.png"
#define BUGZILLA_PICTURE ":/images/bugzilla_icon.png"
#define BSD_BUG_IMAGE ":/images/fbsd-pr.png"
#define KDE_IMAGE ":/images/kde.png"
#define LXDE_IMAGE ":/images/lxde.png"
#define GITHUB_IMAGE ":/images/github.png"

#define FREEBSD_SEND_PR "https://bugs.freebsd.org/bugzilla/enter_bug.cgi", BSD_BUG_IMAGE
#define FREEBSD_BUGS_MAILLIST  "freebsd-bugs", "http://lists.freebsd.org/mailman/listinfo/freebsd-bugs", "freebsd-bugs@freebsd.org"
#define PCBSD_TRAC "https://bugs.pcbsd.org" , TRAC_PICTURE
#define PCBSD_TESTING_MAILLIST "testing", "http://lists.pcbsd.org/mailman/listinfo/testing", "testing@lists.pcbsd.org"

#define XORG_BUGZILLA "https://bugs.freedesktop.org/", BUGZILLA_PICTURE
#define XORG_MAILLIST "xorg", "http://lists.freedesktop.org/archives/xorg/", "xorg@freedesktop.org"
#define FREEBSD_X11_MAILLIST "freebsd-x11", "http://http://lists.freebsd.org/mailman/listinfo/freebsd-x11", "freebsd-x11@freebsd.org"

#define FREEBSD_GNOME_MAILLIST "freebsd-gnome", "http://lists.freebsd.org/mailman/listinfo/freebsd-gnome", "freebsd-gnome@freebsd.org"

#define KDE_BUGTRACKER "http://bugs.kde.org", KDE_IMAGE
#define KDE_FREEBSD_MAILLIST "kde-freebsd", "https://mail.kde.org/mailman/listinfo/kde-freebsd", "kde-freebsd@kde.org"

#define XFCE_BUGZILLA "https://bugzilla.xfce.org/",BUGZILLA_PICTURE
#define FREEBSD_XFCE_MAILLIST "freebsd-xfce", "http://lists.freebsd.org/mailman/listinfo/freebsd-xfce", "freebsd-xfce@freebsd.org"

#define LXDE_BUGTRACKER "http://sourceforge.net/tracker/?group_id=180858", LXDE_IMAGE

#define FREEBSD_PORTS_BUGS_MAILLIST "freebsd-ports-bugs", "http://lists.freebsd.org/mailman/listinfo/freebsd-ports-bugs", "freebsd-ports-bugs@freebsd.org"

#define PCBSD_PBI_BUGS_MAILLIST "pbi-bugs", "http://lists.pcbsd.org/mailman/listinfo/pbi-bugs", "pbi-bugs@lists.pcbsd.org"

#define MATE_GITHUB_URL "https://github.com/mate-desktop", GITHUB_IMAGE

#define CINNAMON_BUGTRACKER_GITHUB "https://github.com/linuxmint/Cinnamon/issues", GITHUB_IMAGE

#endif // REPORT_PLACES_H
