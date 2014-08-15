#-------------------------------------------------
#
# Project created by QtCreator 2012-01-20T09:59:27
#
#-------------------------------------------------

QT       += core gui

TARGET = pc-bluetoothmanager
target.path = /usr/local/bin/
TEMPLATE = app
LANGUAGE = C++

SOURCES += main.cpp\
        btmaingui.cpp

HEADERS  += btmaingui.h

FORMS    += btmaingui.ui

LIBS     += -L../libpcbsd -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head

#RESOURCES += pc-bluetoothmanager.qrc

INCLUDEPATH += ../libpcbsd/ui ../libpcbsd/utils /usr/local/include

TRANSLATIONS =  i18n/pc-bluetoothmanager_af.ts \
                i18n/pc-bluetoothmanager_ar.ts \
                i18n/pc-bluetoothmanager_az.ts \
                i18n/pc-bluetoothmanager_bg.ts \
                i18n/pc-bluetoothmanager_bn.ts \
                i18n/pc-bluetoothmanager_bs.ts \
                i18n/pc-bluetoothmanager_ca.ts \
                i18n/pc-bluetoothmanager_cs.ts \
                i18n/pc-bluetoothmanager_cy.ts \
                i18n/pc-bluetoothmanager_da.ts \
                i18n/pc-bluetoothmanager_de.ts \
                i18n/pc-bluetoothmanager_el.ts \
                i18n/pc-bluetoothmanager_en_GB.ts \
                i18n/pc-bluetoothmanager_en_ZA.ts \
                i18n/pc-bluetoothmanager_es.ts \
                i18n/pc-bluetoothmanager_et.ts \
                i18n/pc-bluetoothmanager_eu.ts \
                i18n/pc-bluetoothmanager_fa.ts \
                i18n/pc-bluetoothmanager_fi.ts \
                i18n/pc-bluetoothmanager_fr.ts \
                i18n/pc-bluetoothmanager_fr_CA.ts \
                i18n/pc-bluetoothmanager_gl.ts \
                i18n/pc-bluetoothmanager_he.ts \
                i18n/pc-bluetoothmanager_hi.ts \
                i18n/pc-bluetoothmanager_hr.ts \
                i18n/pc-bluetoothmanager_hu.ts \
                i18n/pc-bluetoothmanager_id.ts \
                i18n/pc-bluetoothmanager_is.ts \
                i18n/pc-bluetoothmanager_it.ts \
                i18n/pc-bluetoothmanager_ja.ts \
                i18n/pc-bluetoothmanager_ka.ts \
                i18n/pc-bluetoothmanager_ko.ts \
                i18n/pc-bluetoothmanager_lt.ts \
                i18n/pc-bluetoothmanager_lv.ts \
                i18n/pc-bluetoothmanager_mk.ts \
                i18n/pc-bluetoothmanager_mn.ts \
                i18n/pc-bluetoothmanager_ms.ts \
                i18n/pc-bluetoothmanager_mt.ts \
                i18n/pc-bluetoothmanager_nb.ts \
                i18n/pc-bluetoothmanager_nl.ts \
                i18n/pc-bluetoothmanager_pa.ts \
                i18n/pc-bluetoothmanager_pl.ts \
                i18n/pc-bluetoothmanager_pt.ts \
                i18n/pc-bluetoothmanager_pt_BR.ts \
                i18n/pc-bluetoothmanager_ro.ts \
                i18n/pc-bluetoothmanager_ru.ts \
                i18n/pc-bluetoothmanager_sa.ts \
                i18n/pc-bluetoothmanager_sk.ts \
                i18n/pc-bluetoothmanager_sl.ts \
                i18n/pc-bluetoothmanager_sr.ts \
                i18n/pc-bluetoothmanager_sv.ts \
                i18n/pc-bluetoothmanager_sw.ts \
                i18n/pc-bluetoothmanager_ta.ts \
                i18n/pc-bluetoothmanager_tg.ts \
                i18n/pc-bluetoothmanager_th.ts \
                i18n/pc-bluetoothmanager_tr.ts \
                i18n/pc-bluetoothmanager_uk.ts \
                i18n/pc-bluetoothmanager_ur.ts \
                i18n/pc-bluetoothmanager_uz.ts \
                i18n/pc-bluetoothmanager_vi.ts \
                i18n/pc-bluetoothmanager_zh_CN.ts \
                i18n/pc-bluetoothmanager_zh_HK.ts \
                i18n/pc-bluetoothmanager_zh_TW.ts \
                i18n/pc-bluetoothmanager_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans
