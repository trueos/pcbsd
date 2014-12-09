TEMPLATE        = app
LANGUAGE        = C++
QT += core gui widgets
CONFIG  += qt warn_on release

LIBS += -L/usr/local/lib -lcrypt

SOURCES += group.cpp \
	pcbsdusermanager.cpp \
	simpledlgcode.cpp \
	usermanagerback.cpp \
	changepasscode.cpp \
	maindlgcode.cpp \
	simpleaddcode.cpp \
	user.cpp \ 
        main.cpp \
        adddlgcode.cpp

HEADERS += group.h \
	pcbsdusermanager.h \
	simpledlgcode.h \
	usermanagerback.h \
	changepasscode.h \
	maindlgcode.h \
	simpleaddcode.h \
	user.h \
        adddlgcode.h

FORMS   = SimpleDlg.ui \
	changepassdialog.ui \
	adduser.ui \
	mainDlg.ui \
        pcbsdusermanager.ui \
	simpleadddlg.ui

RESOURCES += pcbsdusermanager.qrc

INCLUDEPATH += /usr/local/include

TARGET = pc-usermanager
target.path = /usr/local/bin/

TRANSLATIONS =  i18n/pcbsdusermanager_af.ts \
		i18n/pcbsdusermanager_ar.ts \
		i18n/pcbsdusermanager_az.ts \
		i18n/pcbsdusermanager_bg.ts \
		i18n/pcbsdusermanager_bn.ts \
		i18n/pcbsdusermanager_bs.ts \
		i18n/pcbsdusermanager_ca.ts \
		i18n/pcbsdusermanager_cs.ts \
		i18n/pcbsdusermanager_cy.ts \
		i18n/pcbsdusermanager_da.ts \
		i18n/pcbsdusermanager_de.ts \
		i18n/pcbsdusermanager_el.ts \
		i18n/pcbsdusermanager_en_GB.ts \
		i18n/pcbsdusermanager_en_ZA.ts \
		i18n/pcbsdusermanager_es.ts \
		i18n/pcbsdusermanager_et.ts \
		i18n/pcbsdusermanager_eu.ts \
		i18n/pcbsdusermanager_fa.ts \
		i18n/pcbsdusermanager_fi.ts \
		i18n/pcbsdusermanager_fr.ts \
		i18n/pcbsdusermanager_fr_CA.ts \
		i18n/pcbsdusermanager_gl.ts \
		i18n/pcbsdusermanager_he.ts \
		i18n/pcbsdusermanager_hi.ts \
		i18n/pcbsdusermanager_hr.ts \
		i18n/pcbsdusermanager_hu.ts \
		i18n/pcbsdusermanager_id.ts \
		i18n/pcbsdusermanager_is.ts \
		i18n/pcbsdusermanager_it.ts \
		i18n/pcbsdusermanager_ja.ts \
		i18n/pcbsdusermanager_ka.ts \
		i18n/pcbsdusermanager_ko.ts \
		i18n/pcbsdusermanager_lt.ts \
		i18n/pcbsdusermanager_lv.ts \
		i18n/pcbsdusermanager_mk.ts \
		i18n/pcbsdusermanager_mn.ts \
		i18n/pcbsdusermanager_ms.ts \
		i18n/pcbsdusermanager_mt.ts \
		i18n/pcbsdusermanager_nb.ts \
		i18n/pcbsdusermanager_nl.ts \
		i18n/pcbsdusermanager_pa.ts \
		i18n/pcbsdusermanager_pl.ts \
		i18n/pcbsdusermanager_pt.ts \
		i18n/pcbsdusermanager_pt_BR.ts \
		i18n/pcbsdusermanager_ro.ts \
		i18n/pcbsdusermanager_ru.ts \
		i18n/pcbsdusermanager_sa.ts \
		i18n/pcbsdusermanager_sk.ts \
		i18n/pcbsdusermanager_sl.ts \
		i18n/pcbsdusermanager_sr.ts \
		i18n/pcbsdusermanager_sv.ts \
		i18n/pcbsdusermanager_sw.ts \
		i18n/pcbsdusermanager_ta.ts \
		i18n/pcbsdusermanager_tg.ts \
		i18n/pcbsdusermanager_th.ts \
		i18n/pcbsdusermanager_tr.ts \
		i18n/pcbsdusermanager_uk.ts \
		i18n/pcbsdusermanager_ur.ts \
		i18n/pcbsdusermanager_uz.ts \
		i18n/pcbsdusermanager_vi.ts \
		i18n/pcbsdusermanager_zh_CN.ts \
		i18n/pcbsdusermanager_zh_HK.ts \
		i18n/pcbsdusermanager_zh_TW.ts \
		i18n/pcbsdusermanager_zu.ts

desktop.path=/usr/local/share/applications/
desktop.files=pcusermanager.desktop

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && /usr/local/lib/qt5/bin/lrelease -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

INSTALLS += target dotrans

QMAKE_LIBDIR = /usr/local/lib/qt5
