TEMPLATE	= app
LANGUAGE	= C++
QT += core gui widgets network
CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils

HEADERS	+= TrayUI.h \
			SysStatus.h \
			dialogReminder.h

SOURCES	+= main.cpp \
			TrayUI.cpp \
			SysStatus.cpp

FORMS		+= dialogReminder.ui

RESOURCES += SystemUpdaterTray.qrc

INCLUDEPATH += ../libpcbsd/utils ../libpcbsd/ui ../libpcbsd /usr/local/include

TRANSLATIONS =  i18n/SystemUpdaterTray_af.ts \
		i18n/SystemUpdaterTray_ar.ts \
		i18n/SystemUpdaterTray_az.ts \
		i18n/SystemUpdaterTray_bg.ts \
		i18n/SystemUpdaterTray_bn.ts \
		i18n/SystemUpdaterTray_bs.ts \
		i18n/SystemUpdaterTray_ca.ts \
		i18n/SystemUpdaterTray_cs.ts \
		i18n/SystemUpdaterTray_cy.ts \
		i18n/SystemUpdaterTray_da.ts \
		i18n/SystemUpdaterTray_de.ts \
		i18n/SystemUpdaterTray_el.ts \
		i18n/SystemUpdaterTray_en_GB.ts \
		i18n/SystemUpdaterTray_en_ZA.ts \
		i18n/SystemUpdaterTray_es.ts \
		i18n/SystemUpdaterTray_et.ts \
		i18n/SystemUpdaterTray_eu.ts \
		i18n/SystemUpdaterTray_fa.ts \
		i18n/SystemUpdaterTray_fi.ts \
		i18n/SystemUpdaterTray_fr.ts \
		i18n/SystemUpdaterTray_fr_CA.ts \
		i18n/SystemUpdaterTray_gl.ts \
		i18n/SystemUpdaterTray_he.ts \
		i18n/SystemUpdaterTray_hi.ts \
		i18n/SystemUpdaterTray_hr.ts \
		i18n/SystemUpdaterTray_hu.ts \
		i18n/SystemUpdaterTray_id.ts \
		i18n/SystemUpdaterTray_is.ts \
		i18n/SystemUpdaterTray_it.ts \
		i18n/SystemUpdaterTray_ja.ts \
		i18n/SystemUpdaterTray_ka.ts \
		i18n/SystemUpdaterTray_ko.ts \
		i18n/SystemUpdaterTray_lt.ts \
		i18n/SystemUpdaterTray_lv.ts \
		i18n/SystemUpdaterTray_mk.ts \
		i18n/SystemUpdaterTray_mn.ts \
		i18n/SystemUpdaterTray_ms.ts \
		i18n/SystemUpdaterTray_mt.ts \
		i18n/SystemUpdaterTray_nb.ts \
		i18n/SystemUpdaterTray_nl.ts \
		i18n/SystemUpdaterTray_pa.ts \
		i18n/SystemUpdaterTray_pl.ts \
		i18n/SystemUpdaterTray_pt.ts \
		i18n/SystemUpdaterTray_pt_BR.ts \
		i18n/SystemUpdaterTray_ro.ts \
		i18n/SystemUpdaterTray_ru.ts \
		i18n/SystemUpdaterTray_sa.ts \
		i18n/SystemUpdaterTray_sk.ts \
		i18n/SystemUpdaterTray_sl.ts \
		i18n/SystemUpdaterTray_sr.ts \
		i18n/SystemUpdaterTray_sv.ts \
		i18n/SystemUpdaterTray_sw.ts \
		i18n/SystemUpdaterTray_ta.ts \
		i18n/SystemUpdaterTray_tg.ts \
		i18n/SystemUpdaterTray_th.ts \
		i18n/SystemUpdaterTray_tr.ts \
		i18n/SystemUpdaterTray_uk.ts \
		i18n/SystemUpdaterTray_ur.ts \
		i18n/SystemUpdaterTray_uz.ts \
		i18n/SystemUpdaterTray_vi.ts \
		i18n/SystemUpdaterTray_zh_CN.ts \
		i18n/SystemUpdaterTray_zh_HK.ts \
		i18n/SystemUpdaterTray_zh_TW.ts \
		i18n/SystemUpdaterTray_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/
desktop.files=pcbsdpbu.desktop

#images.path=/usr/local/share/pcbsd/pc-systemupdatertray/images
#images.files=images/*

TARGET  = pc-systemupdatertray
target.path = /usr/local/bin/

INSTALLS += target dotrans desktop

QMAKE_LIBDIR = /usr/local/lib/qt5
