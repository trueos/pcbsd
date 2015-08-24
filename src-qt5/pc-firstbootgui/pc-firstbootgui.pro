# -------------------------------------------------
# Project created by QtCreator 2009-11-05T01:58:32
# -------------------------------------------------
TARGET = pc-firstboot
target.path = /usr/local/bin/
QT += core gui widgets
LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-utils
INCLUDEPATH+= ../libpcbsd/utils
TEMPLATE = app
SOURCES += main.cpp \
    dialogInfoBox.cpp \
    dialogKeyboard.cpp \
    dialogHelp.cpp \
    firstboot.cpp \
    netKey.cpp \
    backend.cpp \
    services.cpp
    
HEADERS += firstboot.h \
    dialogInfoBox.h \
    dialogKeyboard.h \
    dialogHelp.h \
    helpText.h \
    netKey.h \
    backend.h \
    services.h
    
TRANSLATIONS =  i18n/FirstBoot_af.ts \
		i18n/FirstBoot_ar.ts \
		i18n/FirstBoot_az.ts \
		i18n/FirstBoot_bg.ts \
		i18n/FirstBoot_bn.ts \
		i18n/FirstBoot_bs.ts \
		i18n/FirstBoot_ca.ts \
		i18n/FirstBoot_cs.ts \
		i18n/FirstBoot_cy.ts \
		i18n/FirstBoot_da.ts \
		i18n/FirstBoot_de.ts \
		i18n/FirstBoot_el.ts \
		i18n/FirstBoot_es.ts \
		i18n/FirstBoot_en.ts \
		i18n/FirstBoot_en_US.ts \
		i18n/FirstBoot_en_GB.ts \
		i18n/FirstBoot_en_ZA.ts \
		i18n/FirstBoot_et.ts \
		i18n/FirstBoot_eu.ts \
		i18n/FirstBoot_fa.ts \
		i18n/FirstBoot_fi.ts \
		i18n/FirstBoot_fr.ts \
		i18n/FirstBoot_fr_CA.ts \
		i18n/FirstBoot_fur.ts \
		i18n/FirstBoot_gl.ts \
		i18n/FirstBoot_he.ts \
		i18n/FirstBoot_hi.ts \
		i18n/FirstBoot_hr.ts \
		i18n/FirstBoot_hu.ts \
		i18n/FirstBoot_id.ts \
		i18n/FirstBoot_is.ts \
		i18n/FirstBoot_it.ts \
		i18n/FirstBoot_ja.ts \
		i18n/FirstBoot_ka.ts \
		i18n/FirstBoot_ko.ts \
		i18n/FirstBoot_lt.ts \
		i18n/FirstBoot_lv.ts \
		i18n/FirstBoot_mk.ts \
		i18n/FirstBoot_mn.ts \
		i18n/FirstBoot_ms.ts \
		i18n/FirstBoot_mt.ts \
		i18n/FirstBoot_nb.ts \
		i18n/FirstBoot_ne.ts \
		i18n/FirstBoot_nl.ts \
		i18n/FirstBoot_pa.ts \
		i18n/FirstBoot_pl.ts \
		i18n/FirstBoot_pt.ts \
		i18n/FirstBoot_pt_BR.ts \
		i18n/FirstBoot_ro.ts \
		i18n/FirstBoot_ru.ts \
		i18n/FirstBoot_sa.ts \
		i18n/FirstBoot_sk.ts \
		i18n/FirstBoot_sl.ts \
		i18n/FirstBoot_sr.ts \
		i18n/FirstBoot_sv.ts \
		i18n/FirstBoot_sw.ts \
		i18n/FirstBoot_ta.ts \
		i18n/FirstBoot_tg.ts \
		i18n/FirstBoot_th.ts \
		i18n/FirstBoot_tr.ts \
		i18n/FirstBoot_uk.ts \
		i18n/FirstBoot_ur.ts \
		i18n/FirstBoot_uz.ts \
		i18n/FirstBoot_vi.ts \
		i18n/FirstBoot_zh_CN.ts \
		i18n/FirstBoot_zh_HK.ts \
		i18n/FirstBoot_zh_TW.ts \
		i18n/FirstBoot_zu.ts

INSTALLS += target

FORMS += firstboot.ui dialogHelp.ui dialogKeyboard.ui dialogInfoBox.ui netKey.ui
RESOURCES += pc-firstboot.qrc
