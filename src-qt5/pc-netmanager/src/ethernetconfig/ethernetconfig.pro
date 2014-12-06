TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L../../../libpcbsd -lpcbsd-utils -lpcbsd-ui

HEADERS	+= ethernetconfig.h

SOURCES	+= main.cpp ethernetconfig.cpp

FORMS	= ethernetconfig.ui 

RESOURCES += ethernetconfig.qrc

TRANSLATIONS =  i18n/ethernetconfig_af.ts \
		i18n/ethernetconfig_ar.ts \
		i18n/ethernetconfig_az.ts \
		i18n/ethernetconfig_bg.ts \
		i18n/ethernetconfig_bn.ts \
		i18n/ethernetconfig_bs.ts \
		i18n/ethernetconfig_ca.ts \
		i18n/ethernetconfig_cs.ts \
		i18n/ethernetconfig_cy.ts \
		i18n/ethernetconfig_da.ts \
		i18n/ethernetconfig_de.ts \
		i18n/ethernetconfig_el.ts \
		i18n/ethernetconfig_en_GB.ts \
		i18n/ethernetconfig_en_ZA.ts \
		i18n/ethernetconfig_es.ts \
		i18n/ethernetconfig_et.ts \
		i18n/ethernetconfig_eu.ts \
		i18n/ethernetconfig_fa.ts \
		i18n/ethernetconfig_fi.ts \
		i18n/ethernetconfig_fr.ts \
		i18n/ethernetconfig_fr_CA.ts \
		i18n/ethernetconfig_gl.ts \
		i18n/ethernetconfig_he.ts \
		i18n/ethernetconfig_hi.ts \
		i18n/ethernetconfig_hr.ts \
		i18n/ethernetconfig_hu.ts \
		i18n/ethernetconfig_id.ts \
		i18n/ethernetconfig_is.ts \
		i18n/ethernetconfig_it.ts \
		i18n/ethernetconfig_ja.ts \
		i18n/ethernetconfig_ka.ts \
		i18n/ethernetconfig_ko.ts \
		i18n/ethernetconfig_lt.ts \
		i18n/ethernetconfig_lv.ts \
		i18n/ethernetconfig_mk.ts \
		i18n/ethernetconfig_mn.ts \
		i18n/ethernetconfig_ms.ts \
		i18n/ethernetconfig_mt.ts \
		i18n/ethernetconfig_nb.ts \
		i18n/ethernetconfig_nl.ts \
		i18n/ethernetconfig_pa.ts \
		i18n/ethernetconfig_pl.ts \
		i18n/ethernetconfig_pt.ts \
		i18n/ethernetconfig_pt_BR.ts \
		i18n/ethernetconfig_ro.ts \
		i18n/ethernetconfig_ru.ts \
		i18n/ethernetconfig_sa.ts \
		i18n/ethernetconfig_sk.ts \
		i18n/ethernetconfig_sl.ts \
		i18n/ethernetconfig_sr.ts \
		i18n/ethernetconfig_sv.ts \
		i18n/ethernetconfig_sw.ts \
		i18n/ethernetconfig_ta.ts \
		i18n/ethernetconfig_tg.ts \
		i18n/ethernetconfig_th.ts \
		i18n/ethernetconfig_tr.ts \
		i18n/ethernetconfig_uk.ts \
		i18n/ethernetconfig_ur.ts \
		i18n/ethernetconfig_uz.ts \
		i18n/ethernetconfig_vi.ts \
		i18n/ethernetconfig_zh_CN.ts \
		i18n/ethernetconfig_zh_HK.ts \
		i18n/ethernetconfig_zh_TW.ts \
		i18n/ethernetconfig_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += dotrans target

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

TARGET  = pc-ethernetconfig
target.path = /usr/local/bin

INCLUDEPATH+= ../../../libpcbsd/ui ../../../libpcbsd/utils
