TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L../../../libpcbsd -lpcbsd-utils -lpcbsd-ui

HEADERS	+= NetworkTray.h netKey.h

SOURCES	+= main.cpp NetworkTray.cpp netKey.cpp

FORMS	+= netKey.ui

TRANSLATIONS =  i18n/NetworkTray_af.ts \
		i18n/NetworkTray_ar.ts \
		i18n/NetworkTray_az.ts \
		i18n/NetworkTray_bg.ts \
		i18n/NetworkTray_bn.ts \
		i18n/NetworkTray_bs.ts \
		i18n/NetworkTray_ca.ts \
		i18n/NetworkTray_cs.ts \
		i18n/NetworkTray_cy.ts \
		i18n/NetworkTray_da.ts \
		i18n/NetworkTray_de.ts \
		i18n/NetworkTray_el.ts \
		i18n/NetworkTray_en_GB.ts \
		i18n/NetworkTray_en_ZA.ts \
		i18n/NetworkTray_es.ts \
		i18n/NetworkTray_et.ts \
		i18n/NetworkTray_eu.ts \
		i18n/NetworkTray_fa.ts \
		i18n/NetworkTray_fi.ts \
		i18n/NetworkTray_fr.ts \
		i18n/NetworkTray_fr_CA.ts \
		i18n/NetworkTray_gl.ts \
		i18n/NetworkTray_he.ts \
		i18n/NetworkTray_hi.ts \
		i18n/NetworkTray_hr.ts \
		i18n/NetworkTray_hu.ts \
		i18n/NetworkTray_id.ts \
		i18n/NetworkTray_is.ts \
		i18n/NetworkTray_it.ts \
		i18n/NetworkTray_ja.ts \
		i18n/NetworkTray_ka.ts \
		i18n/NetworkTray_ko.ts \
		i18n/NetworkTray_lt.ts \
		i18n/NetworkTray_lv.ts \
		i18n/NetworkTray_mk.ts \
		i18n/NetworkTray_mn.ts \
		i18n/NetworkTray_ms.ts \
		i18n/NetworkTray_mt.ts \
		i18n/NetworkTray_nb.ts \
		i18n/NetworkTray_nl.ts \
		i18n/NetworkTray_pa.ts \
		i18n/NetworkTray_pl.ts \
		i18n/NetworkTray_pt.ts \
		i18n/NetworkTray_pt_BR.ts \
		i18n/NetworkTray_ro.ts \
		i18n/NetworkTray_ru.ts \
		i18n/NetworkTray_sa.ts \
		i18n/NetworkTray_sk.ts \
		i18n/NetworkTray_sl.ts \
		i18n/NetworkTray_sr.ts \
		i18n/NetworkTray_sv.ts \
		i18n/NetworkTray_sw.ts \
		i18n/NetworkTray_ta.ts \
		i18n/NetworkTray_tg.ts \
		i18n/NetworkTray_th.ts \
		i18n/NetworkTray_tr.ts \
		i18n/NetworkTray_uk.ts \
		i18n/NetworkTray_ur.ts \
		i18n/NetworkTray_uz.ts \
		i18n/NetworkTray_vi.ts \
		i18n/NetworkTray_zh_CN.ts \
		i18n/NetworkTray_zh_TW.ts \
		i18n/NetworkTray_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += dotrans target

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

TARGET  = pc-nettray
target.path = /usr/local/bin/

INCLUDEPATH+= ../../../libpcbsd/utils ../../../libpcbsd/ui
