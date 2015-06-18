TEMPLATE	= app
LANGUAGE	= C++
QT += core gui widgets network
CONFIG	+= qt warn_on release

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-utils -lpcbsd-ui

INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

HEADERS	+= mainUI.h \ 
    pkgVulDialog.h \
    updHistoryDialog.h \
    eolDialog.h \
    branchesDialog.h

SOURCES	+= main.cpp mainUI.cpp \ 
    pkgVulDialog.cpp \
    updHistoryDialog.cpp \
    eolDialog.cpp \
    branchesDialog.cpp

FORMS	= mainUI.ui \
    pkgVulDialog.ui \
    updHistoryDialog.ui \
    eolDialog.ui \
    branchesDialog.ui

RESOURCES = pc-updategui.qrc 

TARGET  = pc-updategui
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/UpdateGui_af.ts \
		i18n/UpdateGui_ar.ts \
		i18n/UpdateGui_az.ts \
		i18n/UpdateGui_bg.ts \
		i18n/UpdateGui_bn.ts \
		i18n/UpdateGui_bs.ts \
		i18n/UpdateGui_ca.ts \
		i18n/UpdateGui_cs.ts \
		i18n/UpdateGui_cy.ts \
		i18n/UpdateGui_da.ts \
		i18n/UpdateGui_de.ts \
		i18n/UpdateGui_el.ts \
		i18n/UpdateGui_en_GB.ts \
		i18n/UpdateGui_en_ZA.ts \
		i18n/UpdateGui_es.ts \
		i18n/UpdateGui_et.ts \
		i18n/UpdateGui_eu.ts \
		i18n/UpdateGui_fa.ts \
		i18n/UpdateGui_fi.ts \
		i18n/UpdateGui_fr.ts \
		i18n/UpdateGui_fr_CA.ts \
		i18n/UpdateGui_gl.ts \
		i18n/UpdateGui_he.ts \
		i18n/UpdateGui_hi.ts \
		i18n/UpdateGui_hr.ts \
		i18n/UpdateGui_hu.ts \
		i18n/UpdateGui_id.ts \
		i18n/UpdateGui_is.ts \
		i18n/UpdateGui_it.ts \
		i18n/UpdateGui_ja.ts \
		i18n/UpdateGui_ka.ts \
		i18n/UpdateGui_ko.ts \
		i18n/UpdateGui_lt.ts \
		i18n/UpdateGui_lv.ts \
		i18n/UpdateGui_mk.ts \
		i18n/UpdateGui_mn.ts \
		i18n/UpdateGui_ms.ts \
		i18n/UpdateGui_mt.ts \
		i18n/UpdateGui_nb.ts \
		i18n/UpdateGui_nl.ts \
		i18n/UpdateGui_pa.ts \
		i18n/UpdateGui_pl.ts \
		i18n/UpdateGui_pt.ts \
		i18n/UpdateGui_pt_BR.ts \
		i18n/UpdateGui_ro.ts \
		i18n/UpdateGui_ru.ts \
		i18n/UpdateGui_sa.ts \
		i18n/UpdateGui_sk.ts \
		i18n/UpdateGui_sl.ts \
		i18n/UpdateGui_sr.ts \
		i18n/UpdateGui_sv.ts \
		i18n/UpdateGui_sw.ts \
		i18n/UpdateGui_ta.ts \
		i18n/UpdateGui_tg.ts \
		i18n/UpdateGui_th.ts \
		i18n/UpdateGui_tr.ts \
		i18n/UpdateGui_uk.ts \
		i18n/UpdateGui_ur.ts \
		i18n/UpdateGui_uz.ts \
		i18n/UpdateGui_vi.ts \
		i18n/UpdateGui_zh_CN.ts \
		i18n/UpdateGui_zh_HK.ts \
		i18n/UpdateGui_zh_TW.ts \
		i18n/UpdateGui_zu.ts

icon.path=/usr/local/share/pcbsd/icons/
icon.files=sysupdate.png

desktop.path=/usr/local/share/applications/ 
desktop.files=pc-updategui.desktop

INSTALLS += target icon desktop

QMAKE_LIBDIR = /usr/local/lib/qt5
