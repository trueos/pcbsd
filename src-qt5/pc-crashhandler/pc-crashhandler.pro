TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

TARGET=pbi-crashhandler-gui
target.path=/usr/local/bin/

SOURCES	+= main.cpp crashhandler.cpp details.cpp

FORMS	= crashhandler.ui \
	details.ui

HEADERS += crashhandler.h details.h

RESOURCES = CrashHandler.qrc

TRANSLATIONS = 	i18n/CrashHandler_en.ts \
		i18n/CrashHandler_af.ts \
		i18n/CrashHandler_ar.ts \
		i18n/CrashHandler_az.ts \
		i18n/CrashHandler_bg.ts \
		i18n/CrashHandler_bn.ts \
		i18n/CrashHandler_bs.ts \
		i18n/CrashHandler_ca.ts \
		i18n/CrashHandler_cs.ts \
		i18n/CrashHandler_cy.ts \
		i18n/CrashHandler_da.ts \
		i18n/CrashHandler_de.ts \
		i18n/CrashHandler_el.ts \
		i18n/CrashHandler_en_GB.ts \
		i18n/CrashHandler_en_ZA.ts \
		i18n/CrashHandler_es.ts \
		i18n/CrashHandler_et.ts \
		i18n/CrashHandler_eu.ts \
		i18n/CrashHandler_fa.ts \
		i18n/CrashHandler_fi.ts \
		i18n/CrashHandler_fr.ts \
		i18n/CrashHandler_fr_CA.ts \
		i18n/CrashHandler_fur.ts \
		i18n/CrashHandler_gl.ts \
		i18n/CrashHandler_he.ts \
		i18n/CrashHandler_hi.ts \
		i18n/CrashHandler_hr.ts \
		i18n/CrashHandler_hu.ts \
		i18n/CrashHandler_id.ts \
		i18n/CrashHandler_is.ts \
		i18n/CrashHandler_it.ts \
		i18n/CrashHandler_ja.ts \
		i18n/CrashHandler_ka.ts \
		i18n/CrashHandler_ko.ts \
		i18n/CrashHandler_lt.ts \
		i18n/CrashHandler_lv.ts \
		i18n/CrashHandler_mk.ts \
		i18n/CrashHandler_mn.ts \
		i18n/CrashHandler_ms.ts \
		i18n/CrashHandler_mt.ts \
		i18n/CrashHandler_nb.ts \
		i18n/CrashHandler_ne.ts \
		i18n/CrashHandler_nl.ts \
		i18n/CrashHandler_pa.ts \
		i18n/CrashHandler_pl.ts \
		i18n/CrashHandler_pt.ts \
		i18n/CrashHandler_pt_BR.ts \
		i18n/CrashHandler_ro.ts \
		i18n/CrashHandler_ru.ts \
		i18n/CrashHandler_sa.ts \
		i18n/CrashHandler_sk.ts \
		i18n/CrashHandler_sl.ts \
		i18n/CrashHandler_sr.ts \
		i18n/CrashHandler_sv.ts \
		i18n/CrashHandler_sw.ts \
		i18n/CrashHandler_ta.ts \
		i18n/CrashHandler_tg.ts \
		i18n/CrashHandler_th.ts \
		i18n/CrashHandler_tr.ts \
		i18n/CrashHandler_uk.ts \
		i18n/CrashHandler_ur.ts \
		i18n/CrashHandler_uz.ts \
		i18n/CrashHandler_vi.ts \
		i18n/CrashHandler_zh_CN.ts \
		i18n/CrashHandler_zh_HK.ts \
		i18n/CrashHandler_zh_TW.ts \
		i18n/CrashHandler_zu.ts 

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans

INCLUDEPATH     += /usr/local/include

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
