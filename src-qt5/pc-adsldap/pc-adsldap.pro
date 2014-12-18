TEMPLATE	= app
LANGUAGE	= C++
QT += core widgets network
CONFIG	+= qt warn_on release

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-utils

INCLUDEPATH	+= ../libpcbsd/utils /usr/local/include

HEADERS	+= adsldap.h

SOURCES	+= main.cpp adsldap.cpp

FORMS	= adsldap.ui

RESOURCES = pc-adsldap.qrc

TARGET  = pc-adsldap
target.path=/usr/local/bin/

TRANSLATIONS =  i18n/PCADSLDAP_af.ts \
		i18n/PCADSLDAP_ar.ts \
		i18n/PCADSLDAP_az.ts \
		i18n/PCADSLDAP_bg.ts \
		i18n/PCADSLDAP_bn.ts \
		i18n/PCADSLDAP_bs.ts \
		i18n/PCADSLDAP_ca.ts \
		i18n/PCADSLDAP_cs.ts \
		i18n/PCADSLDAP_cy.ts \
		i18n/PCADSLDAP_da.ts \
		i18n/PCADSLDAP_de.ts \
		i18n/PCADSLDAP_el.ts \
		i18n/PCADSLDAP_en_GB.ts \
		i18n/PCADSLDAP_en_ZA.ts \
		i18n/PCADSLDAP_es.ts \
		i18n/PCADSLDAP_et.ts \
		i18n/PCADSLDAP_eu.ts \
		i18n/PCADSLDAP_fa.ts \
		i18n/PCADSLDAP_fi.ts \
		i18n/PCADSLDAP_fr.ts \
		i18n/PCADSLDAP_fr_CA.ts \
		i18n/PCADSLDAP_gl.ts \
		i18n/PCADSLDAP_he.ts \
		i18n/PCADSLDAP_hi.ts \
		i18n/PCADSLDAP_hr.ts \
		i18n/PCADSLDAP_hu.ts \
		i18n/PCADSLDAP_id.ts \
		i18n/PCADSLDAP_is.ts \
		i18n/PCADSLDAP_it.ts \
		i18n/PCADSLDAP_ja.ts \
		i18n/PCADSLDAP_ka.ts \
		i18n/PCADSLDAP_ko.ts \
		i18n/PCADSLDAP_lt.ts \
		i18n/PCADSLDAP_lv.ts \
		i18n/PCADSLDAP_mk.ts \
		i18n/PCADSLDAP_mn.ts \
		i18n/PCADSLDAP_ms.ts \
		i18n/PCADSLDAP_mt.ts \
		i18n/PCADSLDAP_nb.ts \
		i18n/PCADSLDAP_nl.ts \
		i18n/PCADSLDAP_pa.ts \
		i18n/PCADSLDAP_pl.ts \
		i18n/PCADSLDAP_pt.ts \
		i18n/PCADSLDAP_pt_BR.ts \
		i18n/PCADSLDAP_ro.ts \
		i18n/PCADSLDAP_ru.ts \
		i18n/PCADSLDAP_sa.ts \
		i18n/PCADSLDAP_sk.ts \
		i18n/PCADSLDAP_sl.ts \
		i18n/PCADSLDAP_sr.ts \
		i18n/PCADSLDAP_sv.ts \
		i18n/PCADSLDAP_sw.ts \
		i18n/PCADSLDAP_ta.ts \
		i18n/PCADSLDAP_tg.ts \
		i18n/PCADSLDAP_th.ts \
		i18n/PCADSLDAP_tr.ts \
		i18n/PCADSLDAP_uk.ts \
		i18n/PCADSLDAP_ur.ts \
		i18n/PCADSLDAP_uz.ts \
		i18n/PCADSLDAP_vi.ts \
		i18n/PCADSLDAP_zh_CN.ts \
		i18n/PCADSLDAP_zh_HK.ts \
		i18n/PCADSLDAP_zh_TW.ts \
		i18n/PCADSLDAP_zu.ts

INSTALLS += target

QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib

