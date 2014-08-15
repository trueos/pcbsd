TEMPLATE	= app
LANGUAGE	= C++

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-utils

INCLUDEPATH+= ../libpcbsd/utils/

CONFIG	+= qt warn_on release

TRANSLATIONS = 	i18n/ServiceManager_af.ts \
		i18n/ServiceManager_ar.ts \
		i18n/ServiceManager_az.ts \
		i18n/ServiceManager_bg.ts \
		i18n/ServiceManager_bn.ts \
		i18n/ServiceManager_bs.ts \
		i18n/ServiceManager_ca.ts \
		i18n/ServiceManager_cs.ts \
		i18n/ServiceManager_cy.ts \
		i18n/ServiceManager_da.ts \
		i18n/ServiceManager_de.ts \
		i18n/ServiceManager_el.ts \
		i18n/ServiceManager_en_GB.ts \
		i18n/ServiceManager_en_ZA.ts \
		i18n/ServiceManager_es.ts \
		i18n/ServiceManager_et.ts \
		i18n/ServiceManager_eu.ts \
		i18n/ServiceManager_fa.ts \
		i18n/ServiceManager_fi.ts \
		i18n/ServiceManager_fr.ts \
		i18n/ServiceManager_fr_CA.ts \
		i18n/ServiceManager_gl.ts \
		i18n/ServiceManager_he.ts \
		i18n/ServiceManager_hi.ts \
		i18n/ServiceManager_hr.ts \
		i18n/ServiceManager_hu.ts \
		i18n/ServiceManager_id.ts \
		i18n/ServiceManager_is.ts \
		i18n/ServiceManager_it.ts \
		i18n/ServiceManager_ja.ts \
		i18n/ServiceManager_ka.ts \
		i18n/ServiceManager_ko.ts \
		i18n/ServiceManager_lt.ts \
		i18n/ServiceManager_lv.ts \
		i18n/ServiceManager_mk.ts \
		i18n/ServiceManager_mn.ts \
		i18n/ServiceManager_ms.ts \
		i18n/ServiceManager_mt.ts \
		i18n/ServiceManager_nb.ts \
		i18n/ServiceManager_nl.ts \
		i18n/ServiceManager_pa.ts \
		i18n/ServiceManager_pl.ts \
		i18n/ServiceManager_pt.ts \
		i18n/ServiceManager_pt_BR.ts \
		i18n/ServiceManager_ro.ts \
		i18n/ServiceManager_ru.ts \
		i18n/ServiceManager_sa.ts \
		i18n/ServiceManager_sk.ts \
		i18n/ServiceManager_sl.ts \
		i18n/ServiceManager_sr.ts \
		i18n/ServiceManager_sv.ts \
		i18n/ServiceManager_sw.ts \
		i18n/ServiceManager_ta.ts \
		i18n/ServiceManager_tg.ts \
		i18n/ServiceManager_th.ts \
		i18n/ServiceManager_tr.ts \
		i18n/ServiceManager_uk.ts \
		i18n/ServiceManager_ur.ts \
		i18n/ServiceManager_uz.ts \
		i18n/ServiceManager_vi.ts \
		i18n/ServiceManager_zh_CN.ts \
		i18n/ServiceManager_zh_HK.ts \
		i18n/ServiceManager_zh_TW.ts \
		i18n/ServiceManager_zu.ts

TARGET  = pc-servicemanager
target.path = /usr/local/bin/


HEADERS	+= progress.h servicemanager.h

SOURCES	+= main.cpp progress.cpp servicemanager.cpp

FORMS	= servicemanager.ui progress.ui

desktop.path=/usr/local/share/applications/
desktop.files=pc-servicemanager.desktop

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans

RESOURCES += servicemanager.qrc
