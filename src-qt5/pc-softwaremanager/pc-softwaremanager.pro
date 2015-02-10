TEMPLATE	= app
LANGUAGE	= C++
QT += core gui widgets network svg network
CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils

INCLUDEPATH+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

HEADERS	+= mainUI.h \
	  migrateUI.h \
    	  smallItemWidget.h \
    	  largeItemWidget.h \
    	  pbiNgBackend.h \
    	  extras.h \
    	  pbiDBAccess.h \
	  updateDialog.h \
	  configDialog.h \
	  ssDialog.h

SOURCES	+= main.cpp \
         mainUI.cpp \
	 migrateUI.cpp \
         pbiNgBackend.cpp \
         pbiDBAccess.cpp \
	 updateDialog.cpp \
	 configDialog.cpp \
	 ssDialog.cpp

RESOURCES += SoftwareManager.qrc

FORMS	= mainUI.ui \
		migrateUI.ui \
		updateDialog.ui \
		configDialog.ui \
		ssDialog.ui

TARGET  = pc-softwaremanager
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/SoftwareManager_af.ts \
		i18n/SoftwareManager_ar.ts \
		i18n/SoftwareManager_az.ts \
		i18n/SoftwareManager_bg.ts \
		i18n/SoftwareManager_bn.ts \
		i18n/SoftwareManager_bs.ts \
		i18n/SoftwareManager_ca.ts \
		i18n/SoftwareManager_cs.ts \
		i18n/SoftwareManager_cy.ts \
		i18n/SoftwareManager_da.ts \
		i18n/SoftwareManager_de.ts \
		i18n/SoftwareManager_el.ts \
		i18n/SoftwareManager_en_GB.ts \
		i18n/SoftwareManager_en_ZA.ts \
		i18n/SoftwareManager_es.ts \
		i18n/SoftwareManager_et.ts \
		i18n/SoftwareManager_eu.ts \
		i18n/SoftwareManager_fa.ts \
		i18n/SoftwareManager_fi.ts \
		i18n/SoftwareManager_fr.ts \
		i18n/SoftwareManager_fr_CA.ts \
		i18n/SoftwareManager_fur.ts \
		i18n/SoftwareManager_gl.ts \
		i18n/SoftwareManager_he.ts \
		i18n/SoftwareManager_hi.ts \
		i18n/SoftwareManager_hr.ts \
		i18n/SoftwareManager_hu.ts \
		i18n/SoftwareManager_id.ts \
		i18n/SoftwareManager_is.ts \
		i18n/SoftwareManager_it.ts \
		i18n/SoftwareManager_ja.ts \
		i18n/SoftwareManager_ka.ts \
		i18n/SoftwareManager_ko.ts \
		i18n/SoftwareManager_lt.ts \
		i18n/SoftwareManager_lv.ts \
		i18n/SoftwareManager_mk.ts \
		i18n/SoftwareManager_mn.ts \
		i18n/SoftwareManager_ms.ts \
		i18n/SoftwareManager_mt.ts \
		i18n/SoftwareManager_nb.ts \
		i18n/SoftwareManager_ne.ts \
		i18n/SoftwareManager_nl.ts \
		i18n/SoftwareManager_pa.ts \
		i18n/SoftwareManager_pl.ts \
		i18n/SoftwareManager_pt.ts \
		i18n/SoftwareManager_pt_BR.ts \
		i18n/SoftwareManager_ro.ts \
		i18n/SoftwareManager_ru.ts \
		i18n/SoftwareManager_sa.ts \
		i18n/SoftwareManager_sk.ts \
		i18n/SoftwareManager_sl.ts \
		i18n/SoftwareManager_sr.ts \
		i18n/SoftwareManager_sv.ts \
		i18n/SoftwareManager_sw.ts \
		i18n/SoftwareManager_ta.ts \
		i18n/SoftwareManager_tg.ts \
		i18n/SoftwareManager_th.ts \
		i18n/SoftwareManager_tr.ts \
		i18n/SoftwareManager_uk.ts \
		i18n/SoftwareManager_ur.ts \
		i18n/SoftwareManager_uz.ts \
		i18n/SoftwareManager_vi.ts \
		i18n/SoftwareManager_zh_CN.ts \
		i18n/SoftwareManager_zh_HK.ts \
		i18n/SoftwareManager_zh_TW.ts \
		i18n/SoftwareManager_zu.ts

icons.path=/usr/local/share/pcbsd/icons
icons.files=icons/appcafe.png

desktop.path=/usr/local/share/applications/
desktop.files=softmanager.desktop

INSTALLS += target icons desktop
