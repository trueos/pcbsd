TEMPLATE	= app
LANGUAGE	= C++
QT += core network widgets

LIBS	+= -L../../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH += ../../libpcbsd/ui /usr/local/include

CONFIG	+= qt warn_on release

HEADERS	+= LPTray.h \
		LPWatcher.h \
		LPMessages.h
		
SOURCES	+= main.cpp \
		LPTray.cpp \
		LPWatcher.cpp \
		LPMessages.cpp

RESOURCES += lPreserve.qrc

FORMS	=  LPMessages.ui

TARGET=life-preserver-tray
target.path=/usr/local/bin


images.path=/usr/local/share/lifePreserver/images/
images.files=images/lifepreserver.png

desktop.path=/usr/local/share/applications
desktop.files=lifepreserver.desktop

desktopperm.path=/usr/local/share/applications/
desktopperm.extra=chmod 644 /usr/local/share/applications/lifepreserver.desktop

INSTALLS += target images

TRANSLATIONS =  i18n/LifePreserverTray_af.ts \
		i18n/LifePreserverTray_ar.ts \
		i18n/LifePreserverTray_az.ts \
		i18n/LifePreserverTray_bg.ts \
		i18n/LifePreserverTray_bn.ts \
		i18n/LifePreserverTray_bs.ts \
		i18n/LifePreserverTray_ca.ts \
		i18n/LifePreserverTray_cs.ts \
		i18n/LifePreserverTray_cy.ts \
		i18n/LifePreserverTray_da.ts \
		i18n/LifePreserverTray_de.ts \
		i18n/LifePreserverTray_el.ts \
		i18n/LifePreserverTray_en_US.ts \
		i18n/LifePreserverTray_en_GB.ts \
		i18n/LifePreserverTray_en_ZA.ts \
		i18n/LifePreserverTray_es.ts \
		i18n/LifePreserverTray_et.ts \
		i18n/LifePreserverTray_eu.ts \
		i18n/LifePreserverTray_fa.ts \
		i18n/LifePreserverTray_fi.ts \
		i18n/LifePreserverTray_fr.ts \
		i18n/LifePreserverTray_fr_CA.ts \
		i18n/LifePreserverTray_fur.ts \
		i18n/LifePreserverTray_gl.ts \
		i18n/LifePreserverTray_he.ts \
		i18n/LifePreserverTray_hi.ts \
		i18n/LifePreserverTray_hr.ts \
		i18n/LifePreserverTray_hu.ts \
		i18n/LifePreserverTray_id.ts \
		i18n/LifePreserverTray_is.ts \
		i18n/LifePreserverTray_it.ts \
		i18n/LifePreserverTray_ja.ts \
		i18n/LifePreserverTray_ka.ts \
		i18n/LifePreserverTray_ko.ts \
		i18n/LifePreserverTray_lt.ts \
		i18n/LifePreserverTray_lv.ts \
		i18n/LifePreserverTray_mk.ts \
		i18n/LifePreserverTray_mn.ts \
		i18n/LifePreserverTray_ms.ts \
		i18n/LifePreserverTray_mt.ts \
		i18n/LifePreserverTray_nb.ts \
		i18n/LifePreserverTray_ne.ts \
		i18n/LifePreserverTray_nl.ts \
		i18n/LifePreserverTray_pa.ts \
		i18n/LifePreserverTray_pl.ts \
		i18n/LifePreserverTray_pt.ts \
		i18n/LifePreserverTray_pt_BR.ts \
		i18n/LifePreserverTray_ro.ts \
		i18n/LifePreserverTray_ru.ts \
		i18n/LifePreserverTray_sa.ts \
		i18n/LifePreserverTray_sk.ts \
		i18n/LifePreserverTray_sl.ts \
		i18n/LifePreserverTray_sr.ts \
		i18n/LifePreserverTray_sv.ts \
		i18n/LifePreserverTray_sw.ts \
		i18n/LifePreserverTray_ta.ts \
		i18n/LifePreserverTray_tg.ts \
		i18n/LifePreserverTray_th.ts \
		i18n/LifePreserverTray_tr.ts \
		i18n/LifePreserverTray_uk.ts \
		i18n/LifePreserverTray_ur.ts \
		i18n/LifePreserverTray_uz.ts \
		i18n/LifePreserverTray_vi.ts \
		i18n/LifePreserverTray_zh_CN.ts \
		i18n/LifePreserverTray_zh_HK.ts \
		i18n/LifePreserverTray_zh_TW.ts \
		i18n/LifePreserverTray_zu.ts

QMAKE_LIBDIR = /usr/local/lib/qt5
