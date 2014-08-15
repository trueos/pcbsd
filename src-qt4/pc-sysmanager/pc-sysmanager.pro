TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils

INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

HEADERS	+= pbsystemtab.h \
	cmdDialog.h \
	longProc.h \
	backend.h

SOURCES	+= main.cpp \
	pbsystemtab.cpp \
	cmdDialog.cpp \
	longProc.cpp \
	backend.cpp
	

FORMS	= pbsystemtab.ui \
	cmdDialog.ui

RESOURCES = PBSystem.qrc 

TARGET  = pc-sysmanager
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/PBSystem_af.ts \
		i18n/PBSystem_ar.ts \
		i18n/PBSystem_az.ts \
		i18n/PBSystem_bg.ts \
		i18n/PBSystem_bn.ts \
		i18n/PBSystem_bs.ts \
		i18n/PBSystem_ca.ts \
		i18n/PBSystem_cs.ts \
		i18n/PBSystem_cy.ts \
		i18n/PBSystem_da.ts \
		i18n/PBSystem_de.ts \
		i18n/PBSystem_el.ts \
		i18n/PBSystem_en_GB.ts \
		i18n/PBSystem_en_ZA.ts \
		i18n/PBSystem_es.ts \
		i18n/PBSystem_et.ts \
		i18n/PBSystem_eu.ts \
		i18n/PBSystem_fa.ts \
		i18n/PBSystem_fi.ts \
		i18n/PBSystem_fr.ts \
		i18n/PBSystem_fr_CA.ts \
		i18n/PBSystem_gl.ts \
		i18n/PBSystem_he.ts \
		i18n/PBSystem_hi.ts \
		i18n/PBSystem_hr.ts \
		i18n/PBSystem_hu.ts \
		i18n/PBSystem_id.ts \
		i18n/PBSystem_is.ts \
		i18n/PBSystem_it.ts \
		i18n/PBSystem_ja.ts \
		i18n/PBSystem_ka.ts \
		i18n/PBSystem_ko.ts \
		i18n/PBSystem_lt.ts \
		i18n/PBSystem_lv.ts \
		i18n/PBSystem_mk.ts \
		i18n/PBSystem_mn.ts \
		i18n/PBSystem_ms.ts \
		i18n/PBSystem_mt.ts \
		i18n/PBSystem_nb.ts \
		i18n/PBSystem_nl.ts \
		i18n/PBSystem_pa.ts \
		i18n/PBSystem_pl.ts \
		i18n/PBSystem_pt.ts \
		i18n/PBSystem_pt_BR.ts \
		i18n/PBSystem_ro.ts \
		i18n/PBSystem_ru.ts \
		i18n/PBSystem_sa.ts \
		i18n/PBSystem_sk.ts \
		i18n/PBSystem_sl.ts \
		i18n/PBSystem_sr.ts \
		i18n/PBSystem_sv.ts \
		i18n/PBSystem_sw.ts \
		i18n/PBSystem_ta.ts \
		i18n/PBSystem_tg.ts \
		i18n/PBSystem_th.ts \
		i18n/PBSystem_tr.ts \
		i18n/PBSystem_uk.ts \
		i18n/PBSystem_ur.ts \
		i18n/PBSystem_uz.ts \
		i18n/PBSystem_vi.ts \
		i18n/PBSystem_zh_CN.ts \
		i18n/PBSystem_zh_HK.ts \
		i18n/PBSystem_zh_TW.ts \
		i18n/PBSystem_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/ 
desktop.files=pc-sysmanager.desktop

scripts.path = /usr/local/share/pcbsd/scripts/
scripts.files = scripts/*

sscreens.path = /usr/local/share/pcbsd/splash-screens/
sscreens.files = splash-screens/*

chmod.path=/usr/local/share/pcbsd/scripts
chmod.extra=cd /usr/local/share/pcbsd/scripts && chmod 755 * 

INSTALLS += target dotrans scripts sscreens chmod

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

QT+= network

