TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L../../../libpcbsd -lpcbsd-utils -lpcbsd-ui

HEADERS	+= dialogwpapersonal.h wepconfig.h wificonfigwidgetbase.h wifiscanssid.h wifiselectiondialog.h

SOURCES	+= dialogwpapersonal.cpp main.cpp wepconfig.cpp wificonfigwidgetbase.cpp wifiscanssid.cpp wifiselectiondialog.cpp

FORMS += dialogwpapersonal.ui wepconfig.ui wificonfigwidgetbase.ui wifiscanssid.ui wifiselectiondialog.ui

RESOURCES += wificonfig.qrc

TRANSLATIONS =  i18n/wificonfig_af.ts \
		i18n/wificonfig_ar.ts \
		i18n/wificonfig_az.ts \
		i18n/wificonfig_bg.ts \
		i18n/wificonfig_bn.ts \
		i18n/wificonfig_bs.ts \
		i18n/wificonfig_ca.ts \
		i18n/wificonfig_cs.ts \
		i18n/wificonfig_cy.ts \
		i18n/wificonfig_da.ts \
		i18n/wificonfig_de.ts \
		i18n/wificonfig_el.ts \
		i18n/wificonfig_en_GB.ts \
		i18n/wificonfig_en_ZA.ts \
		i18n/wificonfig_es.ts \
		i18n/wificonfig_et.ts \
		i18n/wificonfig_eu.ts \
		i18n/wificonfig_fa.ts \
		i18n/wificonfig_fi.ts \
		i18n/wificonfig_fr.ts \
		i18n/wificonfig_fr_CA.ts \
		i18n/wificonfig_gl.ts \
		i18n/wificonfig_he.ts \
		i18n/wificonfig_hi.ts \
		i18n/wificonfig_hr.ts \
		i18n/wificonfig_hu.ts \
		i18n/wificonfig_id.ts \
		i18n/wificonfig_is.ts \
		i18n/wificonfig_it.ts \
		i18n/wificonfig_ja.ts \
		i18n/wificonfig_ka.ts \
		i18n/wificonfig_ko.ts \
		i18n/wificonfig_lt.ts \
		i18n/wificonfig_lv.ts \
		i18n/wificonfig_mk.ts \
		i18n/wificonfig_mn.ts \
		i18n/wificonfig_ms.ts \
		i18n/wificonfig_mt.ts \
		i18n/wificonfig_nb.ts \
		i18n/wificonfig_nl.ts \
		i18n/wificonfig_pa.ts \
		i18n/wificonfig_pl.ts \
		i18n/wificonfig_pt.ts \
		i18n/wificonfig_pt_BR.ts \
		i18n/wificonfig_ro.ts \
		i18n/wificonfig_ru.ts \
		i18n/wificonfig_sa.ts \
		i18n/wificonfig_sk.ts \
		i18n/wificonfig_sl.ts \
		i18n/wificonfig_sr.ts \
		i18n/wificonfig_sv.ts \
		i18n/wificonfig_sw.ts \
		i18n/wificonfig_ta.ts \
		i18n/wificonfig_tg.ts \
		i18n/wificonfig_th.ts \
		i18n/wificonfig_tr.ts \
		i18n/wificonfig_uk.ts \
		i18n/wificonfig_ur.ts \
		i18n/wificonfig_uz.ts \
		i18n/wificonfig_vi.ts \
		i18n/wificonfig_zh_CN.ts \
		i18n/wificonfig_zh_HK.ts \
		i18n/wificonfig_zh_TW.ts \
		i18n/wificonfig_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += dotrans target

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

TARGET  = pc-wificonfig
target.path = /usr/local/bin

INCLUDEPATH+= ../../../libpcbsd/utils ../../../libpcbsd/ui
