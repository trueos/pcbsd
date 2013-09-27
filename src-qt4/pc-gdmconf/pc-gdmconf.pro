#-------------------------------------------------
#
# Project created by QtCreator 2012-02-23T15:41:53
#
#-------------------------------------------------

QT       += core gui

LIBS	 += -lQtSolutions_SingleApplication-head

TARGET = pc-gdmconf
target.path = /usr/local/bin/
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    confedit.cpp

HEADERS  += mainwindow.h \
    confedit.h

FORMS    += mainwindow.ui

RESOURCES += \
    pc-gdmconf.qrc

TRANSLATIONS =  i18n/pc-gdmconf_af.ts \
                i18n/pc-gdmconf_ar.ts \
                i18n/pc-gdmconf_az.ts \
                i18n/pc-gdmconf_bg.ts \
                i18n/pc-gdmconf_bn.ts \
                i18n/pc-gdmconf_bs.ts \
                i18n/pc-gdmconf_ca.ts \
                i18n/pc-gdmconf_cs.ts \
                i18n/pc-gdmconf_cy.ts \
                i18n/pc-gdmconf_da.ts \
                i18n/pc-gdmconf_de.ts \
                i18n/pc-gdmconf_el.ts \
                i18n/pc-gdmconf_es.ts \
                i18n/pc-gdmconf_en_GB.ts \
                i18n/pc-gdmconf_en_ZA.ts \
                i18n/pc-gdmconf_et.ts \
                i18n/pc-gdmconf_eu.ts \
                i18n/pc-gdmconf_fa.ts \
                i18n/pc-gdmconf_fi.ts \
                i18n/pc-gdmconf_fr.ts \
                i18n/pc-gdmconf_fr_CA.ts \
                i18n/pc-gdmconf_fur.ts \
                i18n/pc-gdmconf_gl.ts \
                i18n/pc-gdmconf_he.ts \
                i18n/pc-gdmconf_hi.ts \
                i18n/pc-gdmconf_hr.ts \
                i18n/pc-gdmconf_hu.ts \
                i18n/pc-gdmconf_id.ts \
                i18n/pc-gdmconf_is.ts \
                i18n/pc-gdmconf_it.ts \
                i18n/pc-gdmconf_ja.ts \
                i18n/pc-gdmconf_ka.ts \
                i18n/pc-gdmconf_ko.ts \
                i18n/pc-gdmconf_lt.ts \
                i18n/pc-gdmconf_lv.ts \
                i18n/pc-gdmconf_mk.ts \
                i18n/pc-gdmconf_mn.ts \
                i18n/pc-gdmconf_ms.ts \
                i18n/pc-gdmconf_mt.ts \
                i18n/pc-gdmconf_nb.ts \
                i18n/pc-gdmconf_ne.ts \
                i18n/pc-gdmconf_nl.ts \
                i18n/pc-gdmconf_pa.ts \
                i18n/pc-gdmconf_pl.ts \
                i18n/pc-gdmconf_pt.ts \
                i18n/pc-gdmconf_pt_BR.ts \
                i18n/pc-gdmconf_ro.ts \
                i18n/pc-gdmconf_ru.ts \
                i18n/pc-gdmconf_sa.ts \
                i18n/pc-gdmconf_sk.ts \
                i18n/pc-gdmconf_sl.ts \
                i18n/pc-gdmconf_sr.ts \
                i18n/pc-gdmconf_sv.ts \
                i18n/pc-gdmconf_sw.ts \
                i18n/pc-gdmconf_ta.ts \
                i18n/pc-gdmconf_tg.ts \
                i18n/pc-gdmconf_th.ts \
                i18n/pc-gdmconf_tr.ts \
                i18n/pc-gdmconf_uk.ts \
                i18n/pc-gdmconf_uz.ts \
                i18n/pc-gdmconf_vi.ts \
                i18n/pc-gdmconf_zh_CN.ts \
                i18n/pc-gdmconf_zh_HK.ts \
                i18n/pc-gdmconf_zh_TW.ts \
                i18n/pc-gdmconf_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pcgdmconf.desktop

appicon.path=/usr/local/share/pcbsd/icons
appicon.files=pc-gdmconf.png

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/


INSTALLS += target dotrans appicon desktop


