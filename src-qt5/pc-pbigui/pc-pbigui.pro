TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS += pbi.h

SOURCES	+= main.cpp pbi.cpp

FORMS	+= pbi.ui

RESOURCES += pc-pbigui.qrc

TRANSLATIONS =  i18n/PBI_af.ts \
		i18n/PBI_ar.ts \
		i18n/PBI_az.ts \
		i18n/PBI_bg.ts \
		i18n/PBI_bn.ts \
		i18n/PBI_bs.ts \
		i18n/PBI_ca.ts \
		i18n/PBI_cs.ts \
		i18n/PBI_cy.ts \
		i18n/PBI_da.ts \
		i18n/PBI_de.ts \
		i18n/PBI_el.ts \
		i18n/PBI_es.ts \
		i18n/PBI_et.ts \
		i18n/PBI_eu.ts \
		i18n/PBI_fa.ts \
		i18n/PBI_fi.ts \
		i18n/PBI_fr.ts \
		i18n/PBI_fr_CA.ts \
		i18n/PBI_gl.ts \
		i18n/PBI_he.ts \
		i18n/PBI_hi.ts \
		i18n/PBI_hr.ts \
		i18n/PBI_hu.ts \
		i18n/PBI_id.ts \
		i18n/PBI_is.ts \
		i18n/PBI_it.ts \
		i18n/PBI_ja.ts \
		i18n/PBI_ka.ts \
		i18n/PBI_ko.ts \
		i18n/PBI_lt.ts \
		i18n/PBI_lv.ts \
		i18n/PBI_mk.ts \
		i18n/PBI_mn.ts \
		i18n/PBI_ms.ts \
		i18n/PBI_mt.ts \
		i18n/PBI_nb.ts \
		i18n/PBI_nl.ts \
		i18n/PBI_pa.ts \
		i18n/PBI_pl.ts \
		i18n/PBI_pt.ts \
		i18n/PBI_pt_BR.ts \
		i18n/PBI_ro.ts \
		i18n/PBI_ru.ts \
		i18n/PBI_sa.ts \
		i18n/PBI_sk.ts \
		i18n/PBI_sl.ts \
		i18n/PBI_sr.ts \
		i18n/PBI_sv.ts \
		i18n/PBI_sw.ts \
		i18n/PBI_ta.ts \
		i18n/PBI_tg.ts \
		i18n/PBI_th.ts \
		i18n/PBI_tr.ts \
		i18n/PBI_uk.ts \
		i18n/PBI_ur.ts \
		i18n/PBI_uz.ts \
		i18n/PBI_vi.ts \
		i18n/PBI_zh_CN.ts \
		i18n/PBI_zh_HK.ts \
		i18n/PBI_zh_TW.ts \
		i18n/PBI_zu.ts

dotrans.path=/usr/local/share/pcbsd/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/

dodirs.path=/usr/local/etc/xdg
dodirs.extra=sh mkdirs.sh $(INSTALL_ROOT)

applnk.path=/usr/local/share/applications
applnk.files=resources/pcbsd-pbi.desktop

defaultlist.path=/usr/local/share/applications
defaultlist.files=resources/defaults.list

mimexml.path=/usr/local/share/mime/packages
mimexml.files=resources/pcbsd-pbi.xml

pbiimg.path=/usr/local/share/pcbsd/images/
pbiimg.extra=cd resources && xdg-icon-resource install --context mimetypes --size 64 pcbsd-pbi.png application-pbi

runpbibin.path=/usr/local/bin
runpbibin.files=resources/runpbi

runpbichmod.path=/usr/local/bin
runpbichmod.extra=chmod 755 $(INSTALL_ROOT)/usr/local/bin/runpbi

TARGET  = pc-pbigui
target.path = /usr/local/bin/

INSTALLS += target dodirs dotrans applnk pbiimg mimexml runpbibin runpbichmod defaultlist

