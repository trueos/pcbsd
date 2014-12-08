TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

QT	+= core gui widgets network

LIBS	+= -L../../../libpcbsd -L/usr/local/lib -lpcbsd-utils -lpcbsd-ui
INCLUDEPATH+= ../../../libpcbsd/ui ../../../libpcbsd/utils /usr/local/include

HEADERS	+= networkman.h \
    dnslist.h

SOURCES	+= main.cpp networkman.cpp \
    dnslist.cpp

FORMS	= networkman.ui \
    dnslist.ui

RESOURCES += NetworkManager.qrc 

TARGET  = pc-netmanager
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/NetworkManager_af.ts \
		i18n/NetworkManager_ar.ts \
		i18n/NetworkManager_az.ts \
		i18n/NetworkManager_bn.ts \
		i18n/NetworkManager_bg.ts \
		i18n/NetworkManager_bs.ts \
		i18n/NetworkManager_ca.ts \
		i18n/NetworkManager_cs.ts \
		i18n/NetworkManager_cy.ts \
		i18n/NetworkManager_da.ts \
		i18n/NetworkManager_de.ts \
		i18n/NetworkManager_el.ts \
		i18n/NetworkManager_en_GB.ts \
		i18n/NetworkManager_en_ZA.ts \
		i18n/NetworkManager_es.ts \
		i18n/NetworkManager_et.ts \
		i18n/NetworkManager_eu.ts \
		i18n/NetworkManager_fa.ts \
		i18n/NetworkManager_fi.ts \
		i18n/NetworkManager_fr.ts \
		i18n/NetworkManager_fr_CA.ts \
		i18n/NetworkManager_fur.ts \
		i18n/NetworkManager_gl.ts \
		i18n/NetworkManager_he.ts \
		i18n/NetworkManager_hi.ts \
		i18n/NetworkManager_hr.ts \
		i18n/NetworkManager_hu.ts \
		i18n/NetworkManager_id.ts \
		i18n/NetworkManager_is.ts \
		i18n/NetworkManager_it.ts \
		i18n/NetworkManager_ja.ts \
		i18n/NetworkManager_ka.ts \
		i18n/NetworkManager_ko.ts \
		i18n/NetworkManager_lt.ts \
		i18n/NetworkManager_lv.ts \
		i18n/NetworkManager_mk.ts \
		i18n/NetworkManager_mn.ts \
		i18n/NetworkManager_ms.ts \
		i18n/NetworkManager_mt.ts \
		i18n/NetworkManager_ne.ts \
		i18n/NetworkManager_nb.ts \
		i18n/NetworkManager_nl.ts \
		i18n/NetworkManager_pa.ts \
		i18n/NetworkManager_pl.ts \
		i18n/NetworkManager_pt.ts \
		i18n/NetworkManager_pt_BR.ts \
		i18n/NetworkManager_ro.ts \
		i18n/NetworkManager_ru.ts \
		i18n/NetworkManager_sa.ts \
		i18n/NetworkManager_sk.ts \
		i18n/NetworkManager_sl.ts \
		i18n/NetworkManager_sr.ts \
		i18n/NetworkManager_sv.ts \
		i18n/NetworkManager_sw.ts \
		i18n/NetworkManager_ta.ts \
		i18n/NetworkManager_tg.ts \
		i18n/NetworkManager_th.ts \
		i18n/NetworkManager_tr.ts \
		i18n/NetworkManager_uk.ts \
		i18n/NetworkManager_ur.ts \
		i18n/NetworkManager_uz.ts \
		i18n/NetworkManager_vi.ts \
		i18n/NetworkManager_zh_CN.ts \
		i18n/NetworkManager_zh_HK.ts \
		i18n/NetworkManager_zh_TW.ts \
		i18n/NetworkManager_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pcnetwork.desktop

cpres.path=/usr/local/share/pcbsd/pc-netmanager/
cpres.files=resources/umts_stick

resperm.path=/usr/local/share/pcbsd/pc-netmanager/
resperm.extra=chmod 755 $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-netmanager/umts_stick

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

dnsconf.path=/usr/local/share/pcbsd/conf
dnsconf.files=pubdns.conf

INSTALLS+= target dotrans cpres resperm dnsconf


OTHER_FILES += \
    pubdns.conf
