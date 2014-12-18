TEMPLATE	= app
LANGUAGE	= C++
QT += core gui widgets
CONFIG	+= dll

INCLUDEPATH	+= . config ../libpcbsd/utils/

HEADERS	+= config/packetfilter.hpp \
	config/rcconfig.hpp \
	pfmanagerdlg.hpp \
	config/rule.hpp \
	config/services.hpp \
	config/interfaces.hpp \
	pfaddlg.hpp \
	taggedlistviewitem.hpp

SOURCES	+= config/packetfilter.cpp \
	config/rcconfig.cpp \
	pfmanagerdlg.cpp \
	config/rule.cpp \
	config/services.cpp \
	config/interfaces.cpp \
	pfaddlg.cpp \
	taggedlistviewitem.cpp \
	main.cpp

FORMS	= mainwindow.ui \
	adddlg.ui

RESOURCES += pfmanager.qrc


TARGET = pc-pfmanager
target.path = /usr/local/bin/
DEPENDPATH += config

LIBS    += -L../libpcbsd -L/usr/local/lib -lpcbsd-utils
INCLUDEPATH+= ../libpcbsd/utils /usr/local/include

TRANSLATIONS =  i18n/PFManager_af.ts \
		i18n/PFManager_ar.ts \
		i18n/PFManager_az.ts \
		i18n/PFManager_bg.ts \
		i18n/PFManager_bn.ts \
		i18n/PFManager_bs.ts \
		i18n/PFManager_ca.ts \
		i18n/PFManager_cs.ts \
		i18n/PFManager_cy.ts \
		i18n/PFManager_da.ts \
		i18n/PFManager_de.ts \
		i18n/PFManager_el.ts \
		i18n/PFManager_en_GB.ts \
		i18n/PFManager_en_ZA.ts \
		i18n/PFManager_es.ts \
		i18n/PFManager_et.ts \
		i18n/PFManager_eu.ts \
		i18n/PFManager_fa.ts \
		i18n/PFManager_fi.ts \
		i18n/PFManager_fr.ts \
		i18n/PFManager_fr_CA.ts \
		i18n/PFManager_gl.ts \
		i18n/PFManager_he.ts \
		i18n/PFManager_hi.ts \
		i18n/PFManager_hr.ts \
		i18n/PFManager_hu.ts \
		i18n/PFManager_id.ts \
		i18n/PFManager_is.ts \
		i18n/PFManager_it.ts \
		i18n/PFManager_ja.ts \
		i18n/PFManager_ka.ts \
		i18n/PFManager_ko.ts \
		i18n/PFManager_lt.ts \
		i18n/PFManager_lv.ts \
		i18n/PFManager_mk.ts \
		i18n/PFManager_mn.ts \
		i18n/PFManager_ms.ts \
		i18n/PFManager_mt.ts \
		i18n/PFManager_nb.ts \
		i18n/PFManager_nl.ts \
		i18n/PFManager_pa.ts \
		i18n/PFManager_pl.ts \
		i18n/PFManager_pt.ts \
		i18n/PFManager_pt_BR.ts \
		i18n/PFManager_ro.ts \
		i18n/PFManager_ru.ts \
		i18n/PFManager_sa.ts \
		i18n/PFManager_sk.ts \
		i18n/PFManager_sl.ts \
		i18n/PFManager_sr.ts \
		i18n/PFManager_sv.ts \
		i18n/PFManager_sw.ts \
		i18n/PFManager_ta.ts \
		i18n/PFManager_tg.ts \
		i18n/PFManager_th.ts \
		i18n/PFManager_tr.ts \
		i18n/PFManager_uk.ts \
		i18n/PFManager_ur.ts \
		i18n/PFManager_uz.ts \
		i18n/PFManager_vi.ts \
		i18n/PFManager_zh_CN.ts \
		i18n/PFManager_zh_HK.ts \
		i18n/PFManager_zh_TW.ts \
		i18n/PFManager_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pfmanager.desktop

INSTALLS += target

QMAKE_LIBDIR = /usr/local/lib/qt5
