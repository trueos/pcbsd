
QT       += core gui network

LIBS +=-L../ -L/usr/local/lib -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../utils/ /usr/local/include

TARGET=pcbsd-ui
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_/../

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 0.9.9

HEADERS	+= dialogMetaProgress.h \
	dialogwpaenterprise.h \
	dialogInfoBox.h \
	pcbsd-dialogAPsetup.h \
    pcbsd-ui.h \
    kbsimpleoptionswidget.h \
    kboptionswidget.h \
    kblayoutsetupwidget.h \
    kblayoutselectdialog.h

SOURCES	+= dialogInfoBox.cpp \
	dialogMetaProgress.cpp \
	dialogwpaenterprise.cpp \
	metaWidget.cpp \
	dialogAPsetup.cpp \
    ui.cpp \
    kbsimpleoptionswidget.cpp \
    kboptionswidget.cpp \
    kblayoutsetupwidget.cpp \
    kblayoutselectdialog.cpp

RESOURCES += libpcbsd-ui.qrc

FORMS += dialogInfoBox.ui \
	dialogMetaProgress.ui \
	dialogwpaenterprise.ui \
	dialogAPsetup.ui \
    kbsimpleoptionswidget.ui \
    kboptionswidget.ui \
    kblayoutsetupwidget.ui \
    kblayoutselectdialog.ui

TRANSLATIONS =  i18n/libpcbsd_af.ts \
		i18n/libpcbsd_ar.ts \
		i18n/libpcbsd_az.ts \
		i18n/libpcbsd_bg.ts \
		i18n/libpcbsd_bn.ts \
		i18n/libpcbsd_bs.ts \
		i18n/libpcbsd_ca.ts \
		i18n/libpcbsd_cs.ts \
		i18n/libpcbsd_cy.ts \
		i18n/libpcbsd_da.ts \
		i18n/libpcbsd_de.ts \
		i18n/libpcbsd_el.ts \
		i18n/libpcbsd_en_GB.ts \
		i18n/libpcbsd_en_ZA.ts \
		i18n/libpcbsd_es.ts \
		i18n/libpcbsd_et.ts \
		i18n/libpcbsd_eu.ts \
		i18n/libpcbsd_fa.ts \
		i18n/libpcbsd_fi.ts \
		i18n/libpcbsd_fr.ts \
		i18n/libpcbsd_fr_CA.ts \
		i18n/libpcbsd_fur.ts \
		i18n/libpcbsd_gl.ts \
		i18n/libpcbsd_he.ts \
		i18n/libpcbsd_hi.ts \
		i18n/libpcbsd_hr.ts \
		i18n/libpcbsd_hu.ts \
		i18n/libpcbsd_id.ts \
		i18n/libpcbsd_is.ts \
		i18n/libpcbsd_it.ts \
		i18n/libpcbsd_ja.ts \
		i18n/libpcbsd_ka.ts \
		i18n/libpcbsd_ko.ts \
		i18n/libpcbsd_lt.ts \
		i18n/libpcbsd_lv.ts \
		i18n/libpcbsd_mk.ts \
		i18n/libpcbsd_mn.ts \
		i18n/libpcbsd_ms.ts \
		i18n/libpcbsd_mt.ts \
		i18n/libpcbsd_nb.ts \
		i18n/libpcbsd_ne.ts \
		i18n/libpcbsd_nl.ts \
		i18n/libpcbsd_pa.ts \
		i18n/libpcbsd_pl.ts \
		i18n/libpcbsd_pt.ts \
		i18n/libpcbsd_pt_BR.ts \
		i18n/libpcbsd_ro.ts \
		i18n/libpcbsd_ru.ts \
		i18n/libpcbsd_sa.ts \
		i18n/libpcbsd_sk.ts \
		i18n/libpcbsd_sl.ts \
		i18n/libpcbsd_sr.ts \
		i18n/libpcbsd_sv.ts \
		i18n/libpcbsd_sw.ts \
		i18n/libpcbsd_ta.ts \
		i18n/libpcbsd_tg.ts \
		i18n/libpcbsd_th.ts \
		i18n/libpcbsd_tr.ts \
		i18n/libpcbsd_ur.ts \
		i18n/libpcbsd_uk.ts \
		i18n/libpcbsd_uz.ts \
		i18n/libpcbsd_vi.ts \
		i18n/libpcbsd_zh_CN.ts \
		i18n/libpcbsd_zh_HK.ts \
		i18n/libpcbsd_zh_TW.ts \
		i18n/libpcbsd_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

include.path=/usr/local/include/
include.files=pcbsd-*.h

INSTALLS += target include dotrans

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
