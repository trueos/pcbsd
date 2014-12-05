TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L/usr/local/lib 

INCLUDEPATH	+= /usr/local/include

HEADERS	+= welcomeWizard.h

SOURCES	+= main.cpp welcomeWizard.cpp

FORMS	= welcomeWizard.ui

RESOURCES = pc-welcome.qrc 

TARGET  = pc-welcome
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/PCWelcome_af.ts \
		i18n/PCWelcome_ar.ts \
		i18n/PCWelcome_az.ts \
		i18n/PCWelcome_bg.ts \
		i18n/PCWelcome_bn.ts \
		i18n/PCWelcome_bs.ts \
		i18n/PCWelcome_ca.ts \
		i18n/PCWelcome_cs.ts \
		i18n/PCWelcome_cy.ts \
		i18n/PCWelcome_da.ts \
		i18n/PCWelcome_de.ts \
		i18n/PCWelcome_el.ts \
		i18n/PCWelcome_en_GB.ts \
		i18n/PCWelcome_en_ZA.ts \
		i18n/PCWelcome_es.ts \
		i18n/PCWelcome_et.ts \
		i18n/PCWelcome_eu.ts \
		i18n/PCWelcome_fa.ts \
		i18n/PCWelcome_fi.ts \
		i18n/PCWelcome_fr.ts \
		i18n/PCWelcome_fr_CA.ts \
		i18n/PCWelcome_gl.ts \
		i18n/PCWelcome_he.ts \
		i18n/PCWelcome_hi.ts \
		i18n/PCWelcome_hr.ts \
		i18n/PCWelcome_hu.ts \
		i18n/PCWelcome_id.ts \
		i18n/PCWelcome_is.ts \
		i18n/PCWelcome_it.ts \
		i18n/PCWelcome_ja.ts \
		i18n/PCWelcome_ka.ts \
		i18n/PCWelcome_ko.ts \
		i18n/PCWelcome_lt.ts \
		i18n/PCWelcome_lv.ts \
		i18n/PCWelcome_mk.ts \
		i18n/PCWelcome_mn.ts \
		i18n/PCWelcome_ms.ts \
		i18n/PCWelcome_mt.ts \
		i18n/PCWelcome_nb.ts \
		i18n/PCWelcome_nl.ts \
		i18n/PCWelcome_pa.ts \
		i18n/PCWelcome_pl.ts \
		i18n/PCWelcome_pt.ts \
		i18n/PCWelcome_pt_BR.ts \
		i18n/PCWelcome_ro.ts \
		i18n/PCWelcome_ru.ts \
		i18n/PCWelcome_sa.ts \
		i18n/PCWelcome_sk.ts \
		i18n/PCWelcome_sl.ts \
		i18n/PCWelcome_sr.ts \
		i18n/PCWelcome_sv.ts \
		i18n/PCWelcome_sw.ts \
		i18n/PCWelcome_ta.ts \
		i18n/PCWelcome_tg.ts \
		i18n/PCWelcome_th.ts \
		i18n/PCWelcome_tr.ts \
		i18n/PCWelcome_uk.ts \
		i18n/PCWelcome_ur.ts \
		i18n/PCWelcome_uz.ts \
		i18n/PCWelcome_vi.ts \
		i18n/PCWelcome_zh_CN.ts \
		i18n/PCWelcome_zh_HK.ts \
		i18n/PCWelcome_zh_TW.ts \
		i18n/PCWelcome_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/ 
desktop.files=pc-welcome.desktop

INSTALLS += target dotrans desktop

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

