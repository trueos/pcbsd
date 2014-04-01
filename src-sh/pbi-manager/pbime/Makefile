
all: pbime

pbime: pbime.c pbirun.c pbild.c
	${CC} -lutil -o pbime pbime.c
	${CC} -lutil -o pbirun pbirun.c
	${CC} -mtune=i386 -march=i386 -m32 -lutil -o pbirun32 pbirun.c
	${CC} -lutil -o pbild pbild.c

install: pbime
	install -o root -g wheel -m 755 pbime /usr/pbi/.pbime
	install -o root -g wheel -m 4751 pbild /usr/pbi/.pbild
	install -o root -g wheel -m 755 pbirun /usr/pbi/.pbirun
	install -o root -g wheel -m 755 pbirun32 /usr/pbi/.pbirun32
	install -o root -g wheel -m 755 pbiinit /usr/pbi/.pbiinit
	install -o root -g wheel -m 755 ldconfig /usr/pbi/.ldconfig
	install -o root -g wheel -m 755 pbisyscmd /usr/bin/pbisyscmd
	install -o root -g wheel -m 755 openwith /usr/bin/openwith

clean:
	rm pbirun
	rm pbirun32
	rm pbime
	rm pbild
