TEMPLATE	= app
QT += core gui widgets svg
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-utils -lpcbsd-ui -lpoppler-qt5

INCLUDEPATH+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include /usr/local/include/poppler/qt5

HEADERS	+= pdfUI.h

SOURCES	+= main.cpp \
         pdfUI.cpp

FORMS += pdfUI.ui

RESOURCES += PDFViewer.qrc


TARGET  = pc-pdfviewer
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/pdfviewer_af.ts \
		i18n/pdfviewer_ar.ts \
		i18n/pdfviewer_az.ts \
		i18n/pdfviewer_bg.ts \
		i18n/pdfviewer_bn.ts \
		i18n/pdfviewer_bs.ts \
		i18n/pdfviewer_ca.ts \
		i18n/pdfviewer_cs.ts \
		i18n/pdfviewer_cy.ts \
		i18n/pdfviewer_da.ts \
		i18n/pdfviewer_de.ts \
		i18n/pdfviewer_el.ts \
		i18n/pdfviewer_en_GB.ts \
		i18n/pdfviewer_en_ZA.ts \
		i18n/pdfviewer_es.ts \
		i18n/pdfviewer_et.ts \
		i18n/pdfviewer_eu.ts \
		i18n/pdfviewer_fa.ts \
		i18n/pdfviewer_fi.ts \
		i18n/pdfviewer_fr.ts \
		i18n/pdfviewer_fr_CA.ts \
		i18n/pdfviewer_fur.ts \
		i18n/pdfviewer_gl.ts \
		i18n/pdfviewer_he.ts \
		i18n/pdfviewer_hi.ts \
		i18n/pdfviewer_hr.ts \
		i18n/pdfviewer_hu.ts \
		i18n/pdfviewer_id.ts \
		i18n/pdfviewer_is.ts \
		i18n/pdfviewer_it.ts \
		i18n/pdfviewer_ja.ts \
		i18n/pdfviewer_ka.ts \
		i18n/pdfviewer_ko.ts \
		i18n/pdfviewer_lt.ts \
		i18n/pdfviewer_lv.ts \
		i18n/pdfviewer_mk.ts \
		i18n/pdfviewer_mn.ts \
		i18n/pdfviewer_ms.ts \
		i18n/pdfviewer_mt.ts \
		i18n/pdfviewer_nb.ts \
		i18n/pdfviewer_ne.ts \
		i18n/pdfviewer_nl.ts \
		i18n/pdfviewer_pa.ts \
		i18n/pdfviewer_pl.ts \
		i18n/pdfviewer_pt.ts \
		i18n/pdfviewer_pt_BR.ts \
		i18n/pdfviewer_ro.ts \
		i18n/pdfviewer_ru.ts \
		i18n/pdfviewer_sa.ts \
		i18n/pdfviewer_sk.ts \
		i18n/pdfviewer_sl.ts \
		i18n/pdfviewer_sr.ts \
		i18n/pdfviewer_sv.ts \
		i18n/pdfviewer_sw.ts \
		i18n/pdfviewer_ta.ts \
		i18n/pdfviewer_tg.ts \
		i18n/pdfviewer_th.ts \
		i18n/pdfviewer_tr.ts \
		i18n/pdfviewer_uk.ts \
		i18n/pdfviewer_ur.ts \
		i18n/pdfviewer_uz.ts \
		i18n/pdfviewer_vi.ts \
		i18n/pdfviewer_zh_CN.ts \
		i18n/pdfviewer_zh_HK.ts \
		i18n/pdfviewer_zh_TW.ts \
		i18n/pdfviewer_zu.ts


INSTALLS += target
