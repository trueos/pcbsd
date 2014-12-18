#-------------------------------------------------
#
# Project created by QtCreator 2011-04-19T11:05:08
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = pc-bugreport
target.path = /usr/local/bin

TEMPLATE = app

LIBS +=-L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH	+= ../libpcbsd/ui/ ../libpcbsd/utils/ /usr/local/include

SOURCES += main.cpp\
        mainwindow.cpp \
    maillist.cpp \
    toolbox.cpp \
    showinfodialog.cpp \
    weburl.cpp \
    pbiinfo.cpp

HEADERS  += mainwindow.h \
    maillist.h \
    report_places.h \
    toolbox.h \
    showinfodialog.h \
    weburl.h \
    pbiinfo.h

FORMS    += mainwindow.ui \
    maillist.ui \
    toolbox.ui \
    showinfodialog.ui \
    weburl.ui

RESOURCES += \
    bugreport.qrc

TRANSLATIONS = 	i18n/pc-bugreport_af.ts \
		i18n/pc-bugreport_ar.ts \
		i18n/pc-bugreport_az.ts \
		i18n/pc-bugreport_bg.ts \
		i18n/pc-bugreport_bn.ts \
		i18n/pc-bugreport_bs.ts \
		i18n/pc-bugreport_ca.ts \
		i18n/pc-bugreport_cs.ts \
		i18n/pc-bugreport_cy.ts \
		i18n/pc-bugreport_da.ts \
		i18n/pc-bugreport_de.ts \
		i18n/pc-bugreport_el.ts \
		i18n/pc-bugreport_en_GB.ts \
		i18n/pc-bugreport_en_ZA.ts \
		i18n/pc-bugreport_es.ts \
		i18n/pc-bugreport_et.ts \
		i18n/pc-bugreport_eu.ts \
		i18n/pc-bugreport_fa.ts \
		i18n/pc-bugreport_fi.ts \
		i18n/pc-bugreport_fr.ts \
		i18n/pc-bugreport_fr_CA.ts \
		i18n/pc-bugreport_fur.ts \
		i18n/pc-bugreport_gl.ts \
		i18n/pc-bugreport_he.ts \
		i18n/pc-bugreport_hi.ts \
		i18n/pc-bugreport_hr.ts \
		i18n/pc-bugreport_hu.ts \
		i18n/pc-bugreport_id.ts \
		i18n/pc-bugreport_is.ts \
		i18n/pc-bugreport_it.ts \
		i18n/pc-bugreport_ja.ts \
		i18n/pc-bugreport_ka.ts \
		i18n/pc-bugreport_ko.ts \
		i18n/pc-bugreport_lt.ts \
		i18n/pc-bugreport_lv.ts \
		i18n/pc-bugreport_mk.ts \
		i18n/pc-bugreport_mn.ts \
		i18n/pc-bugreport_ms.ts \
		i18n/pc-bugreport_mt.ts \
		i18n/pc-bugreport_nb.ts \
		i18n/pc-bugreport_ne.ts \
		i18n/pc-bugreport_nl.ts \
		i18n/pc-bugreport_pa.ts \
		i18n/pc-bugreport_pl.ts \
		i18n/pc-bugreport_pt.ts \
		i18n/pc-bugreport_pt_BR.ts \
		i18n/pc-bugreport_ro.ts \
		i18n/pc-bugreport_ru.ts \
		i18n/pc-bugreport_sa.ts \
		i18n/pc-bugreport_sk.ts \
		i18n/pc-bugreport_sl.ts \
		i18n/pc-bugreport_sr.ts \
		i18n/pc-bugreport_sv.ts \
		i18n/pc-bugreport_sw.ts \
		i18n/pc-bugreport_ta.ts \
		i18n/pc-bugreport_tg.ts \
		i18n/pc-bugreport_th.ts \
		i18n/pc-bugreport_tr.ts \
		i18n/pc-bugreport_uk.ts \
		i18n/pc-bugreport_ur.ts \
		i18n/pc-bugreport_uz.ts \
		i18n/pc-bugreport_vi.ts \
		i18n/pc-bugreport_zh_CN.ts \
		i18n/pc-bugreport_zh_HK.ts \
		i18n/pc-bugreport_zh_TW.ts \
		i18n/pc-bugreport_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pcbugreport.desktop

icons.path=/usr/local/share/pcbsd/pc-controlpanel/icons/
icons.files=pc-bugreport.png

INSTALLS += target desktop icons
