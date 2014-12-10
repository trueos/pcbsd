QT       += core gui widgets network

TARGET = pc-bootconfig
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp mainUI.cpp 


HEADERS  += mainUI.h 


FORMS    += mainUI.ui 


RESOURCES+= pc-bootconfig.qrc

LIBS     += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH	 += ../libpcbsd/ui /usr/local/include

TRANSLATIONS =  i18n/pc-bootconfig_af.ts \
                i18n/pc-bootconfig_ar.ts \
                i18n/pc-bootconfig_az.ts \
                i18n/pc-bootconfig_bg.ts \
                i18n/pc-bootconfig_bn.ts \
                i18n/pc-bootconfig_bs.ts \
                i18n/pc-bootconfig_ca.ts \
                i18n/pc-bootconfig_cs.ts \
                i18n/pc-bootconfig_cy.ts \
                i18n/pc-bootconfig_da.ts \
                i18n/pc-bootconfig_de.ts \
                i18n/pc-bootconfig_el.ts \
                i18n/pc-bootconfig_en_GB.ts \
                i18n/pc-bootconfig_en_ZA.ts \
                i18n/pc-bootconfig_es.ts \
                i18n/pc-bootconfig_et.ts \
                i18n/pc-bootconfig_eu.ts \
                i18n/pc-bootconfig_fa.ts \
                i18n/pc-bootconfig_fi.ts \
                i18n/pc-bootconfig_fr.ts \
                i18n/pc-bootconfig_fr_CA.ts \
                i18n/pc-bootconfig_gl.ts \
                i18n/pc-bootconfig_he.ts \
                i18n/pc-bootconfig_hi.ts \
                i18n/pc-bootconfig_hr.ts \
                i18n/pc-bootconfig_hu.ts \
                i18n/pc-bootconfig_id.ts \
                i18n/pc-bootconfig_is.ts \
                i18n/pc-bootconfig_it.ts \
                i18n/pc-bootconfig_ja.ts \
                i18n/pc-bootconfig_ka.ts \
                i18n/pc-bootconfig_ko.ts \
                i18n/pc-bootconfig_lt.ts \
                i18n/pc-bootconfig_lv.ts \
                i18n/pc-bootconfig_mk.ts \
                i18n/pc-bootconfig_mn.ts \
                i18n/pc-bootconfig_ms.ts \
                i18n/pc-bootconfig_mt.ts \
                i18n/pc-bootconfig_nb.ts \
                i18n/pc-bootconfig_nl.ts \
                i18n/pc-bootconfig_pa.ts \
                i18n/pc-bootconfig_pl.ts \
                i18n/pc-bootconfig_pt.ts \
                i18n/pc-bootconfig_pt_BR.ts \
                i18n/pc-bootconfig_ro.ts \
                i18n/pc-bootconfig_ru.ts \
                i18n/pc-bootconfig_sa.ts \
                i18n/pc-bootconfig_sk.ts \
                i18n/pc-bootconfig_sl.ts \
                i18n/pc-bootconfig_sr.ts \
                i18n/pc-bootconfig_sv.ts \
                i18n/pc-bootconfig_sw.ts \
                i18n/pc-bootconfig_ta.ts \
                i18n/pc-bootconfig_tg.ts \
                i18n/pc-bootconfig_th.ts \
                i18n/pc-bootconfig_tr.ts \
                i18n/pc-bootconfig_uk.ts \
                i18n/pc-bootconfig_ur.ts \
                i18n/pc-bootconfig_uz.ts \
                i18n/pc-bootconfig_vi.ts \
                i18n/pc-bootconfig_zh_CN.ts \
                i18n/pc-bootconfig_zh_HK.ts \
                i18n/pc-bootconfig_zh_TW.ts \
                i18n/pc-bootconfig_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

icons = pc-bootconfig.png
icons.path=/usr/local/share/pcbsd/icons/
icons.extra=cp pc-bootconfig.png $(INSTALL_ROOT)/usr/local/share/pcbsd/icons/pc-bootconfig.png

INSTALLS += target icons dotrans
