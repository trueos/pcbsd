#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:11:50
#
#-------------------------------------------------

QT       += core gui

LIBS	 += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-controlpanel
TEMPLATE = app

target.path = /usr/local/bin/

SOURCES += main.cpp\
        mainwindow.cpp \
    backend/cp-item.cpp \
    backend/cp-itemgroup.cpp \
    controls/qautoexpandlist.cpp

HEADERS  += mainwindow.h \
    backend/cp-item.h \
    backend/cp-itemgroup.h \
    backend/misc.h \
    controls/qautoexpandlist.h

FORMS    += mainwindow.ui

TRANSLATIONS =  i18n/pc-controlpanel_af.ts \
                i18n/pc-controlpanel_ar.ts \
                i18n/pc-controlpanel_az.ts \
                i18n/pc-controlpanel_bg.ts \
                i18n/pc-controlpanel_bn.ts \
                i18n/pc-controlpanel_bs.ts \
                i18n/pc-controlpanel_ca.ts \
                i18n/pc-controlpanel_cs.ts \
                i18n/pc-controlpanel_cy.ts \
                i18n/pc-controlpanel_da.ts \
                i18n/pc-controlpanel_de.ts \
                i18n/pc-controlpanel_el.ts \
                i18n/pc-controlpanel_en_GB.ts \
                i18n/pc-controlpanel_en_ZA.ts \
                i18n/pc-controlpanel_es.ts \
                i18n/pc-controlpanel_et.ts \
                i18n/pc-controlpanel_eu.ts \
                i18n/pc-controlpanel_fa.ts \
                i18n/pc-controlpanel_fi.ts \
                i18n/pc-controlpanel_fr.ts \
                i18n/pc-controlpanel_fr_CA.ts \
                i18n/pc-controlpanel_fur.ts \
                i18n/pc-controlpanel_gl.ts \
                i18n/pc-controlpanel_he.ts \
                i18n/pc-controlpanel_hi.ts \
                i18n/pc-controlpanel_hr.ts \
                i18n/pc-controlpanel_hu.ts \
                i18n/pc-controlpanel_id.ts \
                i18n/pc-controlpanel_is.ts \
                i18n/pc-controlpanel_it.ts \
                i18n/pc-controlpanel_ja.ts \
                i18n/pc-controlpanel_ka.ts \
                i18n/pc-controlpanel_ko.ts \
                i18n/pc-controlpanel_lt.ts \
                i18n/pc-controlpanel_lv.ts \
                i18n/pc-controlpanel_mk.ts \
                i18n/pc-controlpanel_mn.ts \
                i18n/pc-controlpanel_ms.ts \
                i18n/pc-controlpanel_mt.ts \
                i18n/pc-controlpanel_nb.ts \
                i18n/pc-controlpanel_ne.ts \
                i18n/pc-controlpanel_nl.ts \
                i18n/pc-controlpanel_pa.ts \
                i18n/pc-controlpanel_pl.ts \
                i18n/pc-controlpanel_pt.ts \
                i18n/pc-controlpanel_pt_BR.ts \
                i18n/pc-controlpanel_ro.ts \
                i18n/pc-controlpanel_ru.ts \
                i18n/pc-controlpanel_sa.ts \
                i18n/pc-controlpanel_sk.ts \
                i18n/pc-controlpanel_sl.ts \
                i18n/pc-controlpanel_sr.ts \
                i18n/pc-controlpanel_sv.ts \
                i18n/pc-controlpanel_sw.ts \
                i18n/pc-controlpanel_ta.ts \
                i18n/pc-controlpanel_tg.ts \
                i18n/pc-controlpanel_th.ts \
                i18n/pc-controlpanel_tr.ts \
                i18n/pc-controlpanel_uk.ts \
                i18n/pc-controlpanel_ur.ts \
                i18n/pc-controlpanel_uz.ts \
                i18n/pc-controlpanel_vi.ts \
                i18n/pc-controlpanel_zh_CN.ts \
                i18n/pc-controlpanel_zh_HK.ts \
                i18n/pc-controlpanel_zh_TW.ts \
                i18n/pc-controlpanel_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/
desktop.files=pccontrol.desktop

cleanitems.path=/usr/local/share/pcbsd/pc-controlpanel/items
cleanitems.extra=rm -rf $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items

mkdiritems.path=/usr/local/share/pcbsd/pc-controlpanel/items
mkdiritems.extra=mkdir -p $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items

cpitems.path=/usr/local/share/pcbsd/pc-controlpanel/items
cpitems.extra=tar cvf - --exclude '.svn/' -C items . 2>/dev/null | tar xvf - -C $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items && chmod -R 0555 $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items 2>/dev/null

mkdiricons.path=/usr/local/share/pcbsd/pc-controlpanel/icons
mkdiricons.extra=mkdir -p $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/icons

cpicons.path=/usr/local/share/pcbsd/pc-controlpanel/items
cpicons.extra=tar cvf - --exclude '.svn/' -C icons . 2>/dev/null | tar xvf - -C $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/icons 2>/dev/null


INSTALLS += target desktop cleanitems mkdiritems cpitems mkdiricons cpicons dotrans

RESOURCES += \
    controlpanel.qrc
