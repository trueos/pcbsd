
QT += core gui widgets network
LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH += ../libpcbsd/ui /usr/local/include

TARGET=pc-mixer
TEMPLATE	= app


CONFIG	+= qt warn_on release

HEADERS	+= MixerBackend.h \
		MixerTray.h  \
		MixerGUI.h \
		DeviceWidget.h
		
SOURCES	+= main.cpp \
		MixerTray.cpp  \
		MixerGUI.cpp \
		DeviceWidget.cpp

RESOURCES += pc-mixer.qrc

FORMS	=  MixerGUI.ui \
		DeviceWidget.ui


target.path=/usr/local/bin

INSTALLS += target

TRANSLATIONS =  i18n/pc-mixer_af.ts \
		i18n/pc-mixer_ar.ts \
		i18n/pc-mixer_az.ts \
		i18n/pc-mixer_bg.ts \
		i18n/pc-mixer_bn.ts \
		i18n/pc-mixer_bs.ts \
		i18n/pc-mixer_ca.ts \
		i18n/pc-mixer_cs.ts \
		i18n/pc-mixer_cy.ts \
		i18n/pc-mixer_da.ts \
		i18n/pc-mixer_de.ts \
		i18n/pc-mixer_el.ts \
		i18n/pc-mixer_en_US.ts \
		i18n/pc-mixer_en_GB.ts \
		i18n/pc-mixer_en_ZA.ts \
		i18n/pc-mixer_es.ts \
		i18n/pc-mixer_et.ts \
		i18n/pc-mixer_eu.ts \
		i18n/pc-mixer_fa.ts \
		i18n/pc-mixer_fi.ts \
		i18n/pc-mixer_fr.ts \
		i18n/pc-mixer_fr_CA.ts \
		i18n/pc-mixer_fur.ts \
		i18n/pc-mixer_gl.ts \
		i18n/pc-mixer_he.ts \
		i18n/pc-mixer_hi.ts \
		i18n/pc-mixer_hr.ts \
		i18n/pc-mixer_hu.ts \
		i18n/pc-mixer_id.ts \
		i18n/pc-mixer_is.ts \
		i18n/pc-mixer_it.ts \
		i18n/pc-mixer_ja.ts \
		i18n/pc-mixer_ka.ts \
		i18n/pc-mixer_ko.ts \
		i18n/pc-mixer_lt.ts \
		i18n/pc-mixer_lv.ts \
		i18n/pc-mixer_mk.ts \
		i18n/pc-mixer_mn.ts \
		i18n/pc-mixer_ms.ts \
		i18n/pc-mixer_mt.ts \
		i18n/pc-mixer_nb.ts \
		i18n/pc-mixer_ne.ts \
		i18n/pc-mixer_nl.ts \
		i18n/pc-mixer_pa.ts \
		i18n/pc-mixer_pl.ts \
		i18n/pc-mixer_pt.ts \
		i18n/pc-mixer_pt_BR.ts \
		i18n/pc-mixer_ro.ts \
		i18n/pc-mixer_ru.ts \
		i18n/pc-mixer_sa.ts \
		i18n/pc-mixer_sk.ts \
		i18n/pc-mixer_sl.ts \
		i18n/pc-mixer_sr.ts \
		i18n/pc-mixer_sv.ts \
		i18n/pc-mixer_sw.ts \
		i18n/pc-mixer_ta.ts \
		i18n/pc-mixer_tg.ts \
		i18n/pc-mixer_th.ts \
		i18n/pc-mixer_tr.ts \
		i18n/pc-mixer_uk.ts \
		i18n/pc-mixer_ur.ts \
		i18n/pc-mixer_uz.ts \
		i18n/pc-mixer_vi.ts \
		i18n/pc-mixer_zh_CN.ts \
		i18n/pc-mixer_zh_HK.ts \
		i18n/pc-mixer_zh_TW.ts \
		i18n/pc-mixer_zu.ts

QMAKE_LIBDIR = /usr/local/lib/qt5
