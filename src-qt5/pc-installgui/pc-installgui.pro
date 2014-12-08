# -------------------------------------------------
# Project created by QtCreator 2009-11-05T01:58:32
# -------------------------------------------------
TARGET = pc-sysinstaller
target.path = /usr/local/bin/
TEMPLATE = app
QT += core gui widgets
LIBS += -L/usr/local/lib
SOURCES += main.cpp \
    dialogCheckHardware.cpp \
    dialogDesktopSelection.cpp \
    dialogFSSize.cpp \
    dialogInfoBox.cpp \
    dialogKeyboard.cpp \
    dialogHelp.cpp \
    wizardDisk.cpp \
    wizardFreeBSD.cpp \
    wizardRestore.cpp \
    installer.cpp \
    backend.cpp
HEADERS += installer.h \
    dialogCheckHardware.h \
    dialogDesktopSelection.h \
    dialogFSSize.h \
    dialogInfoBox.h \
    dialogKeyboard.h \
    dialogHelp.h \
    wizardDisk.h \
    wizardFreeBSD.h \
    wizardRestore.h \
    helpText.h \
    backend.h
TRANSLATIONS =  i18n/SysInstaller_af.ts \
		i18n/SysInstaller_ar.ts \
		i18n/SysInstaller_az.ts \
		i18n/SysInstaller_bg.ts \
		i18n/SysInstaller_bn.ts \
		i18n/SysInstaller_bs.ts \
		i18n/SysInstaller_ca.ts \
		i18n/SysInstaller_cs.ts \
		i18n/SysInstaller_cy.ts \
		i18n/SysInstaller_da.ts \
		i18n/SysInstaller_de.ts \
		i18n/SysInstaller_el.ts \
		i18n/SysInstaller_es.ts \
		i18n/SysInstaller_en.ts \
		i18n/SysInstaller_en_US.ts \
		i18n/SysInstaller_en_GB.ts \
		i18n/SysInstaller_en_ZA.ts \
		i18n/SysInstaller_et.ts \
		i18n/SysInstaller_eu.ts \
		i18n/SysInstaller_fa.ts \
		i18n/SysInstaller_fi.ts \
		i18n/SysInstaller_fr.ts \
		i18n/SysInstaller_fr_CA.ts \
		i18n/SysInstaller_fur.ts \
		i18n/SysInstaller_gl.ts \
		i18n/SysInstaller_he.ts \
		i18n/SysInstaller_hi.ts \
		i18n/SysInstaller_hr.ts \
		i18n/SysInstaller_hu.ts \
		i18n/SysInstaller_id.ts \
		i18n/SysInstaller_is.ts \
		i18n/SysInstaller_it.ts \
		i18n/SysInstaller_ja.ts \
		i18n/SysInstaller_ka.ts \
		i18n/SysInstaller_ko.ts \
		i18n/SysInstaller_lt.ts \
		i18n/SysInstaller_lv.ts \
		i18n/SysInstaller_mk.ts \
		i18n/SysInstaller_mn.ts \
		i18n/SysInstaller_ms.ts \
		i18n/SysInstaller_mt.ts \
		i18n/SysInstaller_nb.ts \
		i18n/SysInstaller_ne.ts \
		i18n/SysInstaller_nl.ts \
		i18n/SysInstaller_pa.ts \
		i18n/SysInstaller_pl.ts \
		i18n/SysInstaller_pt.ts \
		i18n/SysInstaller_pt_BR.ts \
		i18n/SysInstaller_ro.ts \
		i18n/SysInstaller_ru.ts \
		i18n/SysInstaller_sa.ts \
		i18n/SysInstaller_sk.ts \
		i18n/SysInstaller_sl.ts \
		i18n/SysInstaller_sr.ts \
		i18n/SysInstaller_sv.ts \
		i18n/SysInstaller_sw.ts \
		i18n/SysInstaller_ta.ts \
		i18n/SysInstaller_tg.ts \
		i18n/SysInstaller_th.ts \
		i18n/SysInstaller_tr.ts \
		i18n/SysInstaller_uk.ts \
		i18n/SysInstaller_ur.ts \
		i18n/SysInstaller_uz.ts \
		i18n/SysInstaller_vi.ts \
		i18n/SysInstaller_zh_CN.ts \
		i18n/SysInstaller_zh_HK.ts \
		i18n/SysInstaller_zh_TW.ts \
		i18n/SysInstaller_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

scripts.path=/usr/local/share/pcbsd/pc-installgui
scripts.extra=cp scripts/* $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-installgui/ && chmod 755 $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-installgui/*.sh

INSTALLS += target dotrans scripts

FORMS += installer.ui dialogCheckHardware.ui dialogDesktopSelection.ui dialogFSSize.ui dialogHelp.ui wizardFreeBSD.ui dialogKeyboard.ui wizardDisk.ui dialogInfoBox.ui wizardRestore.ui

RESOURCES += sysinstaller.qrc
