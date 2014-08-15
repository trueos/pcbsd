TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

QMAKE_LIBDIR = /usr/local/lib/qt4

SOURCES	+= main.cpp xgui.cpp dialogconfirm.cpp

FORMS	= xgui.ui \
	dialogconfirm.ui

TRANSLATIONS =  i18n/XGUISource_af.ts \
		i18n/XGUISource_ar.ts \
		i18n/XGUISource_az.ts \
		i18n/XGUISource_bg.ts \
		i18n/XGUISource_bn.ts \
		i18n/XGUISource_bs.ts \
		i18n/XGUISource_ca.ts \
		i18n/XGUISource_cs.ts \
		i18n/XGUISource_cy.ts \
		i18n/XGUISource_da.ts \
		i18n/XGUISource_de.ts \
		i18n/XGUISource_el.ts \
		i18n/XGUISource_en_GB.ts \
		i18n/XGUISource_en_ZA.ts \
		i18n/XGUISource_es.ts \
		i18n/XGUISource_et.ts \
		i18n/XGUISource_eu.ts \
		i18n/XGUISource_fa.ts \
		i18n/XGUISource_fi.ts \
		i18n/XGUISource_fr.ts \
		i18n/XGUISource_fr_CA.ts \
		i18n/XGUISource_gl.ts \
		i18n/XGUISource_he.ts \
		i18n/XGUISource_hi.ts \
		i18n/XGUISource_hr.ts \
		i18n/XGUISource_hu.ts \
		i18n/XGUISource_id.ts \
		i18n/XGUISource_is.ts \
		i18n/XGUISource_it.ts \
		i18n/XGUISource_ja.ts \
		i18n/XGUISource_ka.ts \
		i18n/XGUISource_ko.ts \
		i18n/XGUISource_lt.ts \
		i18n/XGUISource_lv.ts \
		i18n/XGUISource_mk.ts \
		i18n/XGUISource_mn.ts \
		i18n/XGUISource_ms.ts \
		i18n/XGUISource_mt.ts \
		i18n/XGUISource_nb.ts \
		i18n/XGUISource_nl.ts \
		i18n/XGUISource_pa.ts \
		i18n/XGUISource_pl.ts \
		i18n/XGUISource_pt.ts \
		i18n/XGUISource_pt_BR.ts \
		i18n/XGUISource_ro.ts \
		i18n/XGUISource_ru.ts \
		i18n/XGUISource_sa.ts \
		i18n/XGUISource_sk.ts \
		i18n/XGUISource_sl.ts \
		i18n/XGUISource_sr.ts \
		i18n/XGUISource_sv.ts \
		i18n/XGUISource_sw.ts \
		i18n/XGUISource_ta.ts \
		i18n/XGUISource_tg.ts \
		i18n/XGUISource_th.ts \
		i18n/XGUISource_tr.ts \
		i18n/XGUISource_uk.ts \
		i18n/XGUISource_ur.ts \
		i18n/XGUISource_uz.ts \
		i18n/XGUISource_vi.ts \
		i18n/XGUISource_zh_CN.ts \
		i18n/XGUISource_zh_HK.ts \
		i18n/XGUISource_zh_TW.ts \
		i18n/XGUISource_zu.ts

HEADERS += xgui.h dialogconfirm.h

RESOURCES += XGUI.qrc

TARGET  = pc-xgui
target.path = /usr/local/bin/

bin.path = /usr/local/share/pcbsd/xorg-gui/bin/
bin.files = resources/bin/*
scripts.path = /usr/local/share/pcbsd/xorg-gui/scripts/
scripts.files = resources/scripts/*
conf.path = /usr/local/share/pcbsd/xorg-gui/conf/
conf.files = resources/conf/*
fluxbox.path = /usr/local/share/pcbsd/xorg-gui/conf/.fluxbox
fluxbox.files = resources/conf/.fluxbox/*
carddetect.path = /usr/local/share/pcbsd/xorg-gui/cardDetect
carddetect.files = resources/cardDetect/*
settings.path = /usr/local/share/pcbsd/xorg-gui/settings
settings.files = resources/settings/*
temscripts.path = /usr/local/share/pcbsd/xorg-gui/templates/scripts/
temscripts.files = resources/templates/scripts/*
temheader.path = /usr/local/share/pcbsd/xorg-gui/templates/header/
temheader.files = resources/templates/header/*
temdriver.path = /usr/local/share/pcbsd/xorg-gui/templates/driver/
temdriver.files = resources/templates/driver/*
nvdetect.path = /usr/local/share/pcbsd/xorg-gui/nvidia-detect
nvdetect.files = resources/nvidia-detect/*
dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target bin scripts conf fluxbox carddetect settings temscripts temheader temdriver nvdetect dotrans

