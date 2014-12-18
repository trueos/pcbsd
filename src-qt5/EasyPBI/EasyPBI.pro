#-------------------------------------------------
#
# Project created by QtCreator 2011-10-25T20:47:28
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = EasyPBI
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp\
        mainGUI.cpp \
	backend.cpp \
	newModuleDialog.cpp \
	config.cpp \
	preferencesDialog.cpp \
	aboutDialog.cpp \
	PBIModule.cpp \
	ModuleUtils.cpp \
	pkgSelect.cpp \
	bulkModDialog.cpp \
	SConfigDialog.cpp

HEADERS  += mainGUI.h \
	    backend.h \
	    newModuleDialog.h \
	    config.h \
	    preferencesDialog.h \
	    aboutDialog.h \
	    PBIModule.h \
	    ModuleUtils.h \
	    pkgSelect.h \
	    bulkModDialog.h \
	    SConfigDialog.h

FORMS    += mainGUI.ui \
	    newModuleDialog.ui \
	    preferencesDialog.ui \
	    aboutDialog.ui \
	    pkgSelect.ui \
	    bulkModDialog.ui \
	    SConfigDialog.ui

RESOURCES+= EasyPBI.qrc

TRANSLATIONS =  i18n/EasyPBI_af.ts \
                i18n/EasyPBI_ar.ts \
                i18n/EasyPBI_az.ts \
                i18n/EasyPBI_bg.ts \
                i18n/EasyPBI_bn.ts \
                i18n/EasyPBI_bs.ts \
                i18n/EasyPBI_ca.ts \
                i18n/EasyPBI_cs.ts \
                i18n/EasyPBI_cy.ts \
                i18n/EasyPBI_da.ts \
                i18n/EasyPBI_de.ts \
                i18n/EasyPBI_el.ts \
                i18n/EasyPBI_en_GB.ts \
                i18n/EasyPBI_en_ZA.ts \
                i18n/EasyPBI_es.ts \
                i18n/EasyPBI_et.ts \
                i18n/EasyPBI_eu.ts \
                i18n/EasyPBI_fa.ts \
                i18n/EasyPBI_fi.ts \
                i18n/EasyPBI_fr.ts \
                i18n/EasyPBI_fr_CA.ts \
                i18n/EasyPBI_gl.ts \
                i18n/EasyPBI_he.ts \
                i18n/EasyPBI_hi.ts \
                i18n/EasyPBI_hr.ts \
                i18n/EasyPBI_hu.ts \
                i18n/EasyPBI_id.ts \
                i18n/EasyPBI_is.ts \
                i18n/EasyPBI_it.ts \
                i18n/EasyPBI_ja.ts \
                i18n/EasyPBI_ka.ts \
                i18n/EasyPBI_ko.ts \
                i18n/EasyPBI_lt.ts \
                i18n/EasyPBI_lv.ts \
                i18n/EasyPBI_mk.ts \
                i18n/EasyPBI_mn.ts \
                i18n/EasyPBI_ms.ts \
                i18n/EasyPBI_mt.ts \
                i18n/EasyPBI_nb.ts \
                i18n/EasyPBI_nl.ts \
                i18n/EasyPBI_pa.ts \
                i18n/EasyPBI_pl.ts \
                i18n/EasyPBI_pt.ts \
                i18n/EasyPBI_pt_BR.ts \
                i18n/EasyPBI_ro.ts \
                i18n/EasyPBI_ru.ts \
                i18n/EasyPBI_sa.ts \
                i18n/EasyPBI_sk.ts \
                i18n/EasyPBI_sl.ts \
                i18n/EasyPBI_sr.ts \
                i18n/EasyPBI_sv.ts \
                i18n/EasyPBI_sw.ts \
                i18n/EasyPBI_ta.ts \
                i18n/EasyPBI_tg.ts \
                i18n/EasyPBI_th.ts \
                i18n/EasyPBI_tr.ts \
                i18n/EasyPBI_uk.ts \
                i18n/EasyPBI_ur.ts \
                i18n/EasyPBI_uz.ts \
                i18n/EasyPBI_vi.ts \
                i18n/EasyPBI_zh_CN.ts \
                i18n/EasyPBI_zh_HK.ts \
                i18n/EasyPBI_zh_TW.ts \
                i18n/EasyPBI_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=EasyPBI.desktop

icons.path=/usr/local/share/EasyPBI/
icons.files=icons/EasyPBIicon.png

INSTALLS += target desktop icons
