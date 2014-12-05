TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS    += -L../libpcbsd -lQtSolutions_SingleApplication-head -lpcbsd-utils

HEADERS	+= btTray.h

SOURCES	+= main.cpp btTray.cpp

#FORMS	= SystemUpdater.ui \

RESOURCES += pc-bluetoothtray.qrc

INCLUDEPATH += ../libpcbsd/utils /usr/local/include

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib

TARGET  = pc-bluetoothtray
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/pc-bluetoothtray_af.ts \
                i18n/pc-bluetoothtray_ar.ts \
                i18n/pc-bluetoothtray_az.ts \
                i18n/pc-bluetoothtray_bg.ts \
                i18n/pc-bluetoothtray_bn.ts \
                i18n/pc-bluetoothtray_bs.ts \
                i18n/pc-bluetoothtray_ca.ts \
                i18n/pc-bluetoothtray_cs.ts \
                i18n/pc-bluetoothtray_cy.ts \
                i18n/pc-bluetoothtray_da.ts \
                i18n/pc-bluetoothtray_de.ts \
                i18n/pc-bluetoothtray_el.ts \
                i18n/pc-bluetoothtray_en_GB.ts \
                i18n/pc-bluetoothtray_en_ZA.ts \
                i18n/pc-bluetoothtray_es.ts \
                i18n/pc-bluetoothtray_et.ts \
                i18n/pc-bluetoothtray_eu.ts \
                i18n/pc-bluetoothtray_fa.ts \
                i18n/pc-bluetoothtray_fi.ts \
                i18n/pc-bluetoothtray_fr.ts \
                i18n/pc-bluetoothtray_fr_CA.ts \
                i18n/pc-bluetoothtray_gl.ts \
                i18n/pc-bluetoothtray_he.ts \
                i18n/pc-bluetoothtray_hi.ts \
                i18n/pc-bluetoothtray_hr.ts \
                i18n/pc-bluetoothtray_hu.ts \
                i18n/pc-bluetoothtray_id.ts \
                i18n/pc-bluetoothtray_is.ts \
                i18n/pc-bluetoothtray_it.ts \
                i18n/pc-bluetoothtray_ja.ts \
                i18n/pc-bluetoothtray_ka.ts \
                i18n/pc-bluetoothtray_ko.ts \
                i18n/pc-bluetoothtray_lt.ts \
                i18n/pc-bluetoothtray_lv.ts \
                i18n/pc-bluetoothtray_mk.ts \
                i18n/pc-bluetoothtray_mn.ts \
                i18n/pc-bluetoothtray_ms.ts \
                i18n/pc-bluetoothtray_mt.ts \
                i18n/pc-bluetoothtray_nb.ts \
                i18n/pc-bluetoothtray_nl.ts \
                i18n/pc-bluetoothtray_pa.ts \
                i18n/pc-bluetoothtray_pl.ts \
                i18n/pc-bluetoothtray_pt.ts \
                i18n/pc-bluetoothtray_pt_BR.ts \
                i18n/pc-bluetoothtray_ro.ts \
                i18n/pc-bluetoothtray_ru.ts \
                i18n/pc-bluetoothtray_sa.ts \
                i18n/pc-bluetoothtray_sk.ts \
                i18n/pc-bluetoothtray_sl.ts \
                i18n/pc-bluetoothtray_sr.ts \
                i18n/pc-bluetoothtray_sv.ts \
                i18n/pc-bluetoothtray_sw.ts \
                i18n/pc-bluetoothtray_ta.ts \
                i18n/pc-bluetoothtray_tg.ts \
                i18n/pc-bluetoothtray_th.ts \
                i18n/pc-bluetoothtray_tr.ts \
                i18n/pc-bluetoothtray_uk.ts \
                i18n/pc-bluetoothtray_ur.ts \
                i18n/pc-bluetoothtray_uz.ts \
                i18n/pc-bluetoothtray_vi.ts \
                i18n/pc-bluetoothtray_zh_CN.ts \
                i18n/pc-bluetoothtray_zh_HK.ts \
                i18n/pc-bluetoothtray_zh_TW.ts \
                i18n/pc-bluetoothtray_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans
