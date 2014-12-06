TEMPLATE	= app
QT += webkit
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd
LIBS	+= -lQtSolutions_SingleApplication-head -lpcbsd-utils

INCLUDEPATH+= ../libpcbsd/utils/

HEADERS	+= mainUI.h \
		configDlg.h

SOURCES	+= main.cpp \
         mainUI.cpp \
	 configDlg.cpp
	 
FORMS += configDlg.ui

RESOURCES += SoftwareWeb.qrc


TARGET  = pc-softweb
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/SoftwareWeb_af.ts \
		i18n/SoftwareWeb_ar.ts \
		i18n/SoftwareWeb_az.ts \
		i18n/SoftwareWeb_bg.ts \
		i18n/SoftwareWeb_bn.ts \
		i18n/SoftwareWeb_bs.ts \
		i18n/SoftwareWeb_ca.ts \
		i18n/SoftwareWeb_cs.ts \
		i18n/SoftwareWeb_cy.ts \
		i18n/SoftwareWeb_da.ts \
		i18n/SoftwareWeb_de.ts \
		i18n/SoftwareWeb_el.ts \
		i18n/SoftwareWeb_en_GB.ts \
		i18n/SoftwareWeb_en_ZA.ts \
		i18n/SoftwareWeb_es.ts \
		i18n/SoftwareWeb_et.ts \
		i18n/SoftwareWeb_eu.ts \
		i18n/SoftwareWeb_fa.ts \
		i18n/SoftwareWeb_fi.ts \
		i18n/SoftwareWeb_fr.ts \
		i18n/SoftwareWeb_fr_CA.ts \
		i18n/SoftwareWeb_fur.ts \
		i18n/SoftwareWeb_gl.ts \
		i18n/SoftwareWeb_he.ts \
		i18n/SoftwareWeb_hi.ts \
		i18n/SoftwareWeb_hr.ts \
		i18n/SoftwareWeb_hu.ts \
		i18n/SoftwareWeb_id.ts \
		i18n/SoftwareWeb_is.ts \
		i18n/SoftwareWeb_it.ts \
		i18n/SoftwareWeb_ja.ts \
		i18n/SoftwareWeb_ka.ts \
		i18n/SoftwareWeb_ko.ts \
		i18n/SoftwareWeb_lt.ts \
		i18n/SoftwareWeb_lv.ts \
		i18n/SoftwareWeb_mk.ts \
		i18n/SoftwareWeb_mn.ts \
		i18n/SoftwareWeb_ms.ts \
		i18n/SoftwareWeb_mt.ts \
		i18n/SoftwareWeb_nb.ts \
		i18n/SoftwareWeb_ne.ts \
		i18n/SoftwareWeb_nl.ts \
		i18n/SoftwareWeb_pa.ts \
		i18n/SoftwareWeb_pl.ts \
		i18n/SoftwareWeb_pt.ts \
		i18n/SoftwareWeb_pt_BR.ts \
		i18n/SoftwareWeb_ro.ts \
		i18n/SoftwareWeb_ru.ts \
		i18n/SoftwareWeb_sa.ts \
		i18n/SoftwareWeb_sk.ts \
		i18n/SoftwareWeb_sl.ts \
		i18n/SoftwareWeb_sr.ts \
		i18n/SoftwareWeb_sv.ts \
		i18n/SoftwareWeb_sw.ts \
		i18n/SoftwareWeb_ta.ts \
		i18n/SoftwareWeb_tg.ts \
		i18n/SoftwareWeb_th.ts \
		i18n/SoftwareWeb_tr.ts \
		i18n/SoftwareWeb_uk.ts \
		i18n/SoftwareWeb_ur.ts \
		i18n/SoftwareWeb_uz.ts \
		i18n/SoftwareWeb_vi.ts \
		i18n/SoftwareWeb_zh_CN.ts \
		i18n/SoftwareWeb_zh_HK.ts \
		i18n/SoftwareWeb_zh_TW.ts \
		i18n/SoftwareWeb_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

icons.path=/usr/local/share/pcbsd/icons
icons.files=icons/appcafe.png

INSTALLS += target dotrans icons

QT += svg network
