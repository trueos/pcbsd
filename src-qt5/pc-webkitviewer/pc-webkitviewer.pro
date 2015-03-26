TEMPLATE	= app
QT += core gui widgets network webkitwidgets svg
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-utils -lpcbsd-ui

INCLUDEPATH+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

HEADERS	+= mainUI.h

SOURCES	+= main.cpp \
         mainUI.cpp

RESOURCES += WebkitViewer.qrc


TARGET  = pc-webkitviewer
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/webkitviewer_af.ts \
		i18n/webkitviewer_ar.ts \
		i18n/webkitviewer_az.ts \
		i18n/webkitviewer_bg.ts \
		i18n/webkitviewer_bn.ts \
		i18n/webkitviewer_bs.ts \
		i18n/webkitviewer_ca.ts \
		i18n/webkitviewer_cs.ts \
		i18n/webkitviewer_cy.ts \
		i18n/webkitviewer_da.ts \
		i18n/webkitviewer_de.ts \
		i18n/webkitviewer_el.ts \
		i18n/webkitviewer_en_GB.ts \
		i18n/webkitviewer_en_ZA.ts \
		i18n/webkitviewer_es.ts \
		i18n/webkitviewer_et.ts \
		i18n/webkitviewer_eu.ts \
		i18n/webkitviewer_fa.ts \
		i18n/webkitviewer_fi.ts \
		i18n/webkitviewer_fr.ts \
		i18n/webkitviewer_fr_CA.ts \
		i18n/webkitviewer_fur.ts \
		i18n/webkitviewer_gl.ts \
		i18n/webkitviewer_he.ts \
		i18n/webkitviewer_hi.ts \
		i18n/webkitviewer_hr.ts \
		i18n/webkitviewer_hu.ts \
		i18n/webkitviewer_id.ts \
		i18n/webkitviewer_is.ts \
		i18n/webkitviewer_it.ts \
		i18n/webkitviewer_ja.ts \
		i18n/webkitviewer_ka.ts \
		i18n/webkitviewer_ko.ts \
		i18n/webkitviewer_lt.ts \
		i18n/webkitviewer_lv.ts \
		i18n/webkitviewer_mk.ts \
		i18n/webkitviewer_mn.ts \
		i18n/webkitviewer_ms.ts \
		i18n/webkitviewer_mt.ts \
		i18n/webkitviewer_nb.ts \
		i18n/webkitviewer_ne.ts \
		i18n/webkitviewer_nl.ts \
		i18n/webkitviewer_pa.ts \
		i18n/webkitviewer_pl.ts \
		i18n/webkitviewer_pt.ts \
		i18n/webkitviewer_pt_BR.ts \
		i18n/webkitviewer_ro.ts \
		i18n/webkitviewer_ru.ts \
		i18n/webkitviewer_sa.ts \
		i18n/webkitviewer_sk.ts \
		i18n/webkitviewer_sl.ts \
		i18n/webkitviewer_sr.ts \
		i18n/webkitviewer_sv.ts \
		i18n/webkitviewer_sw.ts \
		i18n/webkitviewer_ta.ts \
		i18n/webkitviewer_tg.ts \
		i18n/webkitviewer_th.ts \
		i18n/webkitviewer_tr.ts \
		i18n/webkitviewer_uk.ts \
		i18n/webkitviewer_ur.ts \
		i18n/webkitviewer_uz.ts \
		i18n/webkitviewer_vi.ts \
		i18n/webkitviewer_zh_CN.ts \
		i18n/webkitviewer_zh_HK.ts \
		i18n/webkitviewer_zh_TW.ts \
		i18n/webkitviewer_zu.ts


INSTALLS += target
