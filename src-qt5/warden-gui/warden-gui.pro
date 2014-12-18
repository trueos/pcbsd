TEMPLATE	= app
LANGUAGE	= C++
QT += core gui widgets network
CONFIG	+= qt warn_on release

LIBS    +=-L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH+= ../libpcbsd/ui ../libpcbsd/utils /usr/local/include

HEADERS += dialogwarden.h \
	dialogconfig.h \
	dialogimport.h \
	dialogworking.h \
	dialogdisplayoutput.h \
	wizardNewJail.h \
	dialogEditIP.h \
	dialogTemplates.h

SOURCES	+= main.cpp \
	dialogwarden.cpp \
	dialogconfig.cpp \
	dialogimport.cpp \
	dialogworking.cpp \
	dialogdisplayoutput.cpp \
	wizardNewJail.cpp \
	dialogEditIP.cpp \
	dialogTemplates.cpp

FORMS	= dialogwarden.ui \
	dialogconfig.ui \
        dialogEditIP.ui \
	dialogdisplayoutput.ui \
	dialogimport.ui \
	dialogworking.ui \
	wizardNewJail.ui \
	dialogTemplates.ui

TARGET	= warden-gui

RESOURCES += warden.qrc

target.path=/usr/local/share/warden/bin

TRANSLATIONS =  i18n/Warden_af.ts \
                i18n/Warden_ar.ts \
                i18n/Warden_az.ts \
                i18n/Warden_bg.ts \
                i18n/Warden_bn.ts \
                i18n/Warden_bs.ts \
                i18n/Warden_ca.ts \
                i18n/Warden_cs.ts \
                i18n/Warden_cy.ts \
                i18n/Warden_da.ts \
                i18n/Warden_de.ts \
                i18n/Warden_el.ts \
                i18n/Warden_es.ts \
                i18n/Warden_en_GB.ts \
                i18n/Warden_en_ZA.ts \
                i18n/Warden_et.ts \
                i18n/Warden_eu.ts \
                i18n/Warden_fa.ts \
                i18n/Warden_fi.ts \
                i18n/Warden_fr.ts \
                i18n/Warden_fr_CA.ts \
                i18n/Warden_fur.ts \
                i18n/Warden_gl.ts \
                i18n/Warden_he.ts \
                i18n/Warden_hi.ts \
                i18n/Warden_hr.ts \
                i18n/Warden_hu.ts \
                i18n/Warden_id.ts \
                i18n/Warden_is.ts \
                i18n/Warden_it.ts \
                i18n/Warden_ja.ts \
                i18n/Warden_ka.ts \
                i18n/Warden_ko.ts \
                i18n/Warden_lt.ts \
                i18n/Warden_lv.ts \
                i18n/Warden_mk.ts \
                i18n/Warden_mn.ts \
                i18n/Warden_ms.ts \
                i18n/Warden_mt.ts \
                i18n/Warden_nb.ts \
                i18n/Warden_ne.ts \
                i18n/Warden_nl.ts \
                i18n/Warden_pa.ts \
                i18n/Warden_pl.ts \
                i18n/Warden_pt.ts \
                i18n/Warden_pt_BR.ts \
                i18n/Warden_ro.ts \
                i18n/Warden_ru.ts \
                i18n/Warden_sa.ts \
                i18n/Warden_sk.ts \
                i18n/Warden_sl.ts \
                i18n/Warden_sr.ts \
                i18n/Warden_sv.ts \
                i18n/Warden_sw.ts \
                i18n/Warden_ta.ts \
                i18n/Warden_tg.ts \
                i18n/Warden_th.ts \
                i18n/Warden_tr.ts \
                i18n/Warden_uk.ts \
                i18n/Warden_ur.ts \
                i18n/Warden_uz.ts \
                i18n/Warden_vi.ts \
                i18n/Warden_zh_CN.ts \
                i18n/Warden_zh_HK.ts \
                i18n/Warden_zh_TW.ts \
                i18n/Warden_zu.ts

doicon.path=/usr/local/share/warden/images/
doicon.files=warden.png

desktop.path=/usr/local/share/applications/
desktop.files=warden.desktop

INSTALLS+= target doicon desktop
