
all: pbifs

pbifs: main.c
	${CC} -Wall main.c `pkg-config fuse --cflags --libs` -o pbifs

install: pbifs
	install -o root -g wheel -m 755 pbifs /usr/pbi/.pbifs
