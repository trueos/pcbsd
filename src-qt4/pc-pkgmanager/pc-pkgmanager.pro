TEMPLATE	= app
LANGUAGE	= C++
CONFIG	+= qt warn_on release

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head

INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

HEADERS	+= mainWin.h dialogConfirm.h

SOURCES	+= main.cpp mainWin.cpp dialogConfirm.cpp

FORMS	= mainWin.ui dialogConfirm.ui

RESOURCES = pc-pkgmanager.qrc 

TARGET  = pc-pkgmanager
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/PackageManager_af.ts \
		i18n/PackageManager_ar.ts \
		i18n/PackageManager_az.ts \
		i18n/PackageManager_bg.ts \
		i18n/PackageManager_bn.ts \
		i18n/PackageManager_bs.ts \
		i18n/PackageManager_ca.ts \
		i18n/PackageManager_cs.ts \
		i18n/PackageManager_cy.ts \
		i18n/PackageManager_da.ts \
		i18n/PackageManager_de.ts \
		i18n/PackageManager_el.ts \
		i18n/PackageManager_en_GB.ts \
		i18n/PackageManager_en_ZA.ts \
		i18n/PackageManager_es.ts \
		i18n/PackageManager_et.ts \
		i18n/PackageManager_eu.ts \
		i18n/PackageManager_fa.ts \
		i18n/PackageManager_fi.ts \
		i18n/PackageManager_fr.ts \
		i18n/PackageManager_fr_CA.ts \
		i18n/PackageManager_gl.ts \
		i18n/PackageManager_he.ts \
		i18n/PackageManager_hi.ts \
		i18n/PackageManager_hr.ts \
		i18n/PackageManager_hu.ts \
		i18n/PackageManager_id.ts \
		i18n/PackageManager_is.ts \
		i18n/PackageManager_it.ts \
		i18n/PackageManager_ja.ts \
		i18n/PackageManager_ka.ts \
		i18n/PackageManager_ko.ts \
		i18n/PackageManager_lt.ts \
		i18n/PackageManager_lv.ts \
		i18n/PackageManager_mk.ts \
		i18n/PackageManager_mn.ts \
		i18n/PackageManager_ms.ts \
		i18n/PackageManager_mt.ts \
		i18n/PackageManager_nb.ts \
		i18n/PackageManager_nl.ts \
		i18n/PackageManager_pa.ts \
		i18n/PackageManager_pl.ts \
		i18n/PackageManager_pt.ts \
		i18n/PackageManager_pt_BR.ts \
		i18n/PackageManager_ro.ts \
		i18n/PackageManager_ru.ts \
		i18n/PackageManager_sk.ts \
		i18n/PackageManager_sl.ts \
		i18n/PackageManager_sr.ts \
		i18n/PackageManager_sv.ts \
		i18n/PackageManager_sw.ts \
		i18n/PackageManager_ta.ts \
		i18n/PackageManager_tg.ts \
		i18n/PackageManager_th.ts \
		i18n/PackageManager_tr.ts \
		i18n/PackageManager_uk.ts \
		i18n/PackageManager_uz.ts \
		i18n/PackageManager_vi.ts \
		i18n/PackageManager_zh_CN.ts \
		i18n/PackageManager_zh_HK.ts \
		i18n/PackageManager_zh_TW.ts \
		i18n/PackageManager_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm /usr/local/share/pcbsd/i18n/

desktop.path=/usr/local/share/applications/ 
desktop.files=pc-pkgmanager.desktop

INSTALLS += target dotrans desktop

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
