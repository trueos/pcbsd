#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T01:16:21
#
#-------------------------------------------------

QT       += core gui

LIBS	 += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-syskeyboard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    keyboardsettings.cpp \
    dialogs/kblayoutselectdialog.cpp \
    widgets/kblayoutsetupwidget.cpp \
    widgets/kbsimpleoptionswidget.cpp \
    widgets/kboptionswidget.cpp

HEADERS  += mainwindow.h \
    keyboardsettings.h \
    dialogs/kblayoutselectdialog.h \
    widgets/kblayoutsetupwidget.h \
    widgets/kbsimpleoptionswidget.h \
    widgets/kboptionswidget.h

FORMS    += mainwindow.ui \
    dialogs/kblayoutselectdialog.ui \
    widgets/kblayoutsetupwidget.ui \
    widgets/kbsimpleoptionswidget.ui \
    widgets/kboptionswidget.ui

RESOURCES += \
    syskeyboard.qrc

TRANSLATIONS =  i18n/pc-syskeyboard_af.ts \
                i18n/pc-syskeyboard_ar.ts \
                i18n/pc-syskeyboard_az.ts \
                i18n/pc-syskeyboard_bg.ts \
                i18n/pc-syskeyboard_bn.ts \
                i18n/pc-syskeyboard_bs.ts \
                i18n/pc-syskeyboard_ca.ts \
                i18n/pc-syskeyboard_cs.ts \
                i18n/pc-syskeyboard_cy.ts \
                i18n/pc-syskeyboard_da.ts \
                i18n/pc-syskeyboard_de.ts \
                i18n/pc-syskeyboard_el.ts \
                i18n/pc-syskeyboard_en_GB.ts \
                i18n/pc-syskeyboard_en_ZA.ts \
                i18n/pc-syskeyboard_es.ts \
                i18n/pc-syskeyboard_et.ts \
                i18n/pc-syskeyboard_eu.ts \
                i18n/pc-syskeyboard_fa.ts \
                i18n/pc-syskeyboard_fi.ts \
                i18n/pc-syskeyboard_fr.ts \
                i18n/pc-syskeyboard_fr_CA.ts \
                i18n/pc-syskeyboard_fur.ts \
                i18n/pc-syskeyboard_gl.ts \
                i18n/pc-syskeyboard_he.ts \
                i18n/pc-syskeyboard_hi.ts \
                i18n/pc-syskeyboard_hr.ts \
                i18n/pc-syskeyboard_hu.ts \
                i18n/pc-syskeyboard_id.ts \
                i18n/pc-syskeyboard_is.ts \
                i18n/pc-syskeyboard_it.ts \
                i18n/pc-syskeyboard_ja.ts \
                i18n/pc-syskeyboard_ka.ts \
                i18n/pc-syskeyboard_ko.ts \
                i18n/pc-syskeyboard_lt.ts \
                i18n/pc-syskeyboard_lv.ts \
                i18n/pc-syskeyboard_mk.ts \
                i18n/pc-syskeyboard_mn.ts \
                i18n/pc-syskeyboard_ms.ts \
                i18n/pc-syskeyboard_mt.ts \
                i18n/pc-syskeyboard_nb.ts \
                i18n/pc-syskeyboard_ne.ts \
                i18n/pc-syskeyboard_nl.ts \
                i18n/pc-syskeyboard_pa.ts \
                i18n/pc-syskeyboard_pl.ts \
                i18n/pc-syskeyboard_pt.ts \
                i18n/pc-syskeyboard_pt_BR.ts \
                i18n/pc-syskeyboard_ro.ts \
                i18n/pc-syskeyboard_ru.ts \
                i18n/pc-syskeyboard_sa.ts \
                i18n/pc-syskeyboard_sk.ts \
                i18n/pc-syskeyboard_sl.ts \
                i18n/pc-syskeyboard_sr.ts \
                i18n/pc-syskeyboard_sv.ts \
                i18n/pc-syskeyboard_sw.ts \
                i18n/pc-syskeyboard_ta.ts \
                i18n/pc-syskeyboard_tg.ts \
                i18n/pc-syskeyboard_th.ts \
                i18n/pc-syskeyboard_tr.ts \
                i18n/pc-syskeyboard_uk.ts \
                i18n/pc-syskeyboard_ur.ts \
                i18n/pc-syskeyboard_uz.ts \
                i18n/pc-syskeyboard_vi.ts \
                i18n/pc-syskeyboard_zh_CN.ts \
                i18n/pc-syskeyboard_zh_HK.ts \
                i18n/pc-syskeyboard_zh_TW.ts \
                i18n/pc-syskeyboard_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/
desktop.files=pcsyskeyboard.desktop


INSTALLS += target desktop #dotrans
