#-------------------------------------------------
#
# Project created by QtCreator 2013-05-28T13:01:47
#
#-------------------------------------------------

QT       += core gui widgets network svg

CONFIG += qt warn_on

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

TARGET = pc-zmanager
target.path = /usr/local/bin/

TEMPLATE = app
LANGUAGE	= C++


SOURCES += main.cpp\
        zmanagerwindow.cpp \
    dialogpartition.cpp \
    dialogmount.cpp \
    dialognewpool.cpp \
    dialogname.cpp \
    dialogprop.cpp \
    dialogfscreate.cpp \
    dialogfsprop.cpp

HEADERS  += zmanagerwindow.h \
    dialogpartition.h \
    dialogmount.h \
    dialognewpool.h \
    dialogname.h \
    dialogprop.h \
    dialogfscreate.h \
    dialogfsprop.h

FORMS    += zmanagerwindow.ui \
    dialogpartition.ui \
    dialogmount.ui \
    dialognewpool.ui \
    dialogname.ui \
    dialogprop.ui \
    dialogfscreate.ui \
    dialogfsprop.ui

TRANSLATIONS =  i18n/ZManager_af.ts \
                i18n/ZManager_ar.ts \
                i18n/ZManager_az.ts \
                i18n/ZManager_bg.ts \
                i18n/ZManager_bn.ts \
                i18n/ZManager_bs.ts \
                i18n/ZManager_ca.ts \
                i18n/ZManager_cs.ts \
                i18n/ZManager_cy.ts \
                i18n/ZManager_da.ts \
                i18n/ZManager_de.ts \
                i18n/ZManager_el.ts \
                i18n/ZManager_en_GB.ts \
                i18n/ZManager_en_ZA.ts \
                i18n/ZManager_es.ts \
                i18n/ZManager_et.ts \
                i18n/ZManager_eu.ts \
                i18n/ZManager_fa.ts \
                i18n/ZManager_fi.ts \
                i18n/ZManager_fr.ts \
                i18n/ZManager_fr_CA.ts \
                i18n/ZManager_fur.ts \
                i18n/ZManager_gl.ts \
                i18n/ZManager_he.ts \
                i18n/ZManager_hi.ts \
                i18n/ZManager_hr.ts \
                i18n/ZManager_hu.ts \
                i18n/ZManager_id.ts \
                i18n/ZManager_is.ts \
                i18n/ZManager_it.ts \
                i18n/ZManager_ja.ts \
                i18n/ZManager_ka.ts \
                i18n/ZManager_ko.ts \
                i18n/ZManager_lt.ts \
                i18n/ZManager_lv.ts \
                i18n/ZManager_mk.ts \
                i18n/ZManager_mn.ts \
                i18n/ZManager_ms.ts \
                i18n/ZManager_mt.ts \
                i18n/ZManager_nb.ts \
                i18n/ZManager_ne.ts \
                i18n/ZManager_nl.ts \
                i18n/ZManager_pa.ts \
                i18n/ZManager_pl.ts \
                i18n/ZManager_pt.ts \
                i18n/ZManager_pt_BR.ts \
                i18n/ZManager_ro.ts \
                i18n/ZManager_ru.ts \
                i18n/ZManager_sk.ts \
                i18n/ZManager_sl.ts \
                i18n/ZManager_sr.ts \
                i18n/ZManager_sv.ts \
                i18n/ZManager_sw.ts \
                i18n/ZManager_ta.ts \
                i18n/ZManager_tg.ts \
                i18n/ZManager_th.ts \
                i18n/ZManager_tr.ts \
                i18n/ZManager_uk.ts \
                i18n/ZManager_ur.ts \
                i18n/ZManager_uz.ts \
                i18n/ZManager_vi.ts \
                i18n/ZManager_zh_CN.ts \
                i18n/ZManager_zh_HK.ts \
                i18n/ZManager_zh_TW.ts \
                i18n/ZManager_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

icons.path=/usr/local/share/pcbsd/icons
icons.files=icons/zmanager.png

INSTALLS += target dotrans icons

RESOURCES += \
    zmanagerres.qrc
