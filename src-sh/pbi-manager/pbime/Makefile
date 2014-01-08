
all: pbime

pbime: pbime.c
	${CC} -lutil -o pbime pbime.c

install: pbime
	install -o root -g wheel -m 4751 pbime /usr/pbi/.pbime
	install -o root -g wheel -m 755 pbimount /usr/pbi/.pbimount
	install -o root -g wheel -m 755 ldconfig /usr/pbi/.ldconfig
	install -o root -g wheel -m 755 pbisyslisten /usr/pbi/.pbisyslisten
	install -o root -g wheel -m 755 pbisyscmd /usr/pbi/.pbisyscmd
