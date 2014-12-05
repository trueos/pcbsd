#-------------------------------------------------
#
# Project created by QtCreator 2012-05-17T14:57:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = about-gui
target.path = /usr/local/bin
TEMPLATE = app


LIBS +=-L../libpcbsd -L/usr/local/lib -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils /usr/local/include

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    about-gui.qrc

OTHER_FILES +=

TRANSLATIONS =  i18n/AboutGui_af.ts \
		i18n/AboutGui_ar.ts \
		i18n/AboutGui_az.ts \
		i18n/AboutGui_bg.ts \
		i18n/AboutGui_bn.ts \
		i18n/AboutGui_bs.ts \
		i18n/AboutGui_ca.ts \
		i18n/AboutGui_cs.ts \
		i18n/AboutGui_cy.ts \
		i18n/AboutGui_da.ts \
		i18n/AboutGui_de.ts \
		i18n/AboutGui_el.ts \
		i18n/AboutGui_en_GB.ts \
		i18n/AboutGui_en_ZA.ts \
		i18n/AboutGui_es.ts \
		i18n/AboutGui_et.ts \
		i18n/AboutGui_eu.ts \
		i18n/AboutGui_fa.ts \
		i18n/AboutGui_fi.ts \
		i18n/AboutGui_fr.ts \
		i18n/AboutGui_fr_CA.ts \
		i18n/AboutGui_fur.ts \
		i18n/AboutGui_gl.ts \
		i18n/AboutGui_he.ts \
		i18n/AboutGui_hi.ts \
		i18n/AboutGui_hr.ts \
		i18n/AboutGui_hu.ts \
		i18n/AboutGui_id.ts \
		i18n/AboutGui_is.ts \
		i18n/AboutGui_it.ts \
		i18n/AboutGui_ja.ts \
		i18n/AboutGui_ka.ts \
		i18n/AboutGui_ko.ts \
		i18n/AboutGui_lt.ts \
		i18n/AboutGui_lv.ts \
		i18n/AboutGui_mk.ts \
		i18n/AboutGui_mn.ts \
		i18n/AboutGui_ms.ts \
		i18n/AboutGui_mt.ts \
		i18n/AboutGui_nb.ts \
		i18n/AboutGui_ne.ts \
		i18n/AboutGui_nl.ts \
		i18n/AboutGui_pa.ts \
		i18n/AboutGui_pl.ts \
		i18n/AboutGui_pt.ts \
		i18n/AboutGui_pt_BR.ts \
		i18n/AboutGui_ro.ts \
		i18n/AboutGui_ru.ts \
		i18n/AboutGui_sa.ts \
		i18n/AboutGui_sk.ts \
		i18n/AboutGui_sl.ts \
		i18n/AboutGui_sr.ts \
		i18n/AboutGui_sv.ts \
		i18n/AboutGui_sw.ts \
		i18n/AboutGui_ta.ts \
		i18n/AboutGui_tg.ts \
		i18n/AboutGui_th.ts \
		i18n/AboutGui_tr.ts \
		i18n/AboutGui_uk.ts \
		i18n/AboutGui_ur.ts \
		i18n/AboutGui_uz.ts \
		i18n/AboutGui_vi.ts \
		i18n/AboutGui_zh_CN.ts \
		i18n/AboutGui_zh_HK.ts \
		i18n/AboutGui_zh_TW.ts \
		i18n/AboutGui_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

icons.path=/usr/local/share/pcbsd/icons/
icons.files=about.png

desktop.path=/usr/local/share/applications/
desktop.files=about.desktop

INSTALLS += target icons desktop dotrans
