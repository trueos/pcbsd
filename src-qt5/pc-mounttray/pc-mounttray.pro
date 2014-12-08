TEMPLATE	= app
LANGUAGE	= C++
QT	+= core gui widgets network

CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils

HEADERS	+= mountTray.h \
	menuItem.h \
	devCheck.h \
	fsWatcher.h \
	fsDialog.h \
	settingsDialog.h

SOURCES	+= main.cpp \
	mountTray.cpp \
	menuItem.cpp \
	devCheck.cpp \
	fsWatcher.cpp \
	fsDialog.cpp \
	settingsDialog.cpp

RESOURCES += pc-mounttray.qrc

INCLUDEPATH += ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

QMAKE_LIBDIR = /usr/local/lib/qt5

TRANSLATIONS =  i18n/MountTray_af.ts \
		i18n/MountTray_ar.ts \
		i18n/MountTray_az.ts \
		i18n/MountTray_bg.ts \
		i18n/MountTray_bn.ts \
		i18n/MountTray_bs.ts \
		i18n/MountTray_ca.ts \
		i18n/MountTray_cs.ts \
		i18n/MountTray_cy.ts \
		i18n/MountTray_da.ts \
		i18n/MountTray_de.ts \
		i18n/MountTray_el.ts \
		i18n/MountTray_en_GB.ts \
		i18n/MountTray_en_ZA.ts \
		i18n/MountTray_es.ts \
		i18n/MountTray_et.ts \
		i18n/MountTray_eu.ts \
		i18n/MountTray_fa.ts \
		i18n/MountTray_fi.ts \
		i18n/MountTray_fr.ts \
		i18n/MountTray_fr_CA.ts \
		i18n/MountTray_gl.ts \
		i18n/MountTray_he.ts \
		i18n/MountTray_hi.ts \
		i18n/MountTray_hr.ts \
		i18n/MountTray_hu.ts \
		i18n/MountTray_id.ts \
		i18n/MountTray_is.ts \
		i18n/MountTray_it.ts \
		i18n/MountTray_ja.ts \
		i18n/MountTray_ka.ts \
		i18n/MountTray_ko.ts \
		i18n/MountTray_lt.ts \
		i18n/MountTray_lv.ts \
		i18n/MountTray_mk.ts \
		i18n/MountTray_mn.ts \
		i18n/MountTray_ms.ts \
		i18n/MountTray_mt.ts \
		i18n/MountTray_nb.ts \
		i18n/MountTray_nl.ts \
		i18n/MountTray_pa.ts \
		i18n/MountTray_pl.ts \
		i18n/MountTray_pt.ts \
		i18n/MountTray_pt_BR.ts \
		i18n/MountTray_ro.ts \
		i18n/MountTray_ru.ts \
		i18n/MountTray_sa.ts \
		i18n/MountTray_sk.ts \
		i18n/MountTray_sl.ts \
		i18n/MountTray_sr.ts \
		i18n/MountTray_sv.ts \
		i18n/MountTray_sw.ts \
		i18n/MountTray_ta.ts \
		i18n/MountTray_tg.ts \
		i18n/MountTray_th.ts \
		i18n/MountTray_tr.ts \
		i18n/MountTray_uk.ts \
		i18n/MountTray_ur.ts \
		i18n/MountTray_uz.ts \
		i18n/MountTray_vi.ts \
		i18n/MountTray_zh_CN.ts \
		i18n/MountTray_zh_HK.ts \
		i18n/MountTray_zh_TW.ts \
		i18n/MountTray_zu.ts

TARGET = pc-mounttray
target.path = /usr/local/bin/

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans

