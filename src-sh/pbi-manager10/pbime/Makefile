
all: pbime

pbime: pbime.c
	${CC} -lutil -o pbime pbime.c

install: pbime
	install -o root -g wheel -m 4751 pbime /usr/pbi/.pbime
	install -o root -g wheel -m 755 pbimount /usr/pbi/.pbimount
