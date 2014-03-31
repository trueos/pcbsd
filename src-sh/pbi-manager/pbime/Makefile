
all: pbime

pbime: pbime.c pbirun.c pbild.c
	${CC} -lutil -o pbime pbime.c
	${CC} -lutil -o pbirun pbirun.c
	${CC} -lutil -o pbild pbild.c

install: pbime
	install -o root -g wheel -m 755 pbime /usr/pbi/.pbime
	install -o root -g wheel -m 4751 pbild /usr/pbi/.pbild
	install -o root -g wheel -m 755 pbirun /usr/pbi/.pbirun
	install -o root -g wheel -m 755 pbiinit /usr/pbi/.pbiinit
	install -o root -g wheel -m 755 ldconfig /usr/pbi/.ldconfig
	install -o root -g wheel -m 755 pbisyscmd /usr/pbi/.pbisyscmd
	install -o root -g wheel -m 755 pbisyscmd /usr/bin/openwith
