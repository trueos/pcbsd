#-------------------------------------------------
#
# Project created by QtCreator 2012-02-23T15:41:53
#
#-------------------------------------------------

QT       += core gui widgets network

LIBS	 +=-L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH+= ../libpcbsd/ui ../libpcbsd/utils /usr/local/include


TARGET = pc-dmconf
target.path = /usr/local/bin/
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    pc-dmconf.qrc

TRANSLATIONS =  i18n/pc-dmconf_af.ts \
                i18n/pc-dmconf_ar.ts \
                i18n/pc-dmconf_az.ts \
                i18n/pc-dmconf_bg.ts \
                i18n/pc-dmconf_bn.ts \
                i18n/pc-dmconf_bs.ts \
                i18n/pc-dmconf_ca.ts \
                i18n/pc-dmconf_cs.ts \
                i18n/pc-dmconf_cy.ts \
                i18n/pc-dmconf_da.ts \
                i18n/pc-dmconf_de.ts \
                i18n/pc-dmconf_el.ts \
                i18n/pc-dmconf_es.ts \
                i18n/pc-dmconf_en_GB.ts \
                i18n/pc-dmconf_en_ZA.ts \
                i18n/pc-dmconf_et.ts \
                i18n/pc-dmconf_eu.ts \
                i18n/pc-dmconf_fa.ts \
                i18n/pc-dmconf_fi.ts \
                i18n/pc-dmconf_fr.ts \
                i18n/pc-dmconf_fr_CA.ts \
                i18n/pc-dmconf_fur.ts \
                i18n/pc-dmconf_gl.ts \
                i18n/pc-dmconf_he.ts \
                i18n/pc-dmconf_hi.ts \
                i18n/pc-dmconf_hr.ts \
                i18n/pc-dmconf_hu.ts \
                i18n/pc-dmconf_id.ts \
                i18n/pc-dmconf_is.ts \
                i18n/pc-dmconf_it.ts \
                i18n/pc-dmconf_ja.ts \
                i18n/pc-dmconf_ka.ts \
                i18n/pc-dmconf_ko.ts \
                i18n/pc-dmconf_lt.ts \
                i18n/pc-dmconf_lv.ts \
                i18n/pc-dmconf_mk.ts \
                i18n/pc-dmconf_mn.ts \
                i18n/pc-dmconf_ms.ts \
                i18n/pc-dmconf_mt.ts \
                i18n/pc-dmconf_nb.ts \
                i18n/pc-dmconf_ne.ts \
                i18n/pc-dmconf_nl.ts \
                i18n/pc-dmconf_pa.ts \
                i18n/pc-dmconf_pl.ts \
                i18n/pc-dmconf_pt.ts \
                i18n/pc-dmconf_pt_BR.ts \
                i18n/pc-dmconf_ro.ts \
                i18n/pc-dmconf_ru.ts \
                i18n/pc-dmconf_sa.ts \
                i18n/pc-dmconf_sk.ts \
                i18n/pc-dmconf_sl.ts \
                i18n/pc-dmconf_sr.ts \
                i18n/pc-dmconf_sv.ts \
                i18n/pc-dmconf_sw.ts \
                i18n/pc-dmconf_ta.ts \
                i18n/pc-dmconf_tg.ts \
                i18n/pc-dmconf_th.ts \
                i18n/pc-dmconf_tr.ts \
                i18n/pc-dmconf_uk.ts \
                i18n/pc-dmconf_ur.ts \
                i18n/pc-dmconf_uz.ts \
                i18n/pc-dmconf_vi.ts \
                i18n/pc-dmconf_zh_CN.ts \
                i18n/pc-dmconf_zh_HK.ts \
                i18n/pc-dmconf_zh_TW.ts \
                i18n/pc-dmconf_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pc-dmconf.desktop

appicon.path=/usr/local/share/pcbsd/icons
appicon.files=icons/pc-dmconf.png

INSTALLS += target appicon desktop
