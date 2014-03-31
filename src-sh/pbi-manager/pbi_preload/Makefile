
all: pbi_preload

pbi_preload: pbi_preload.c
	${CC} -fPIC -c -o pbi_preload.o pbi_preload.c
	${CC} -shared -o pbi_preload.so pbi_preload.o 

install: pbi_preload
	install -o root -g wheel -m 755 pbi_preload.so /usr/pbi/.pbi_preload.so

clean:
	rm pbi_preload.so
	rm pbi_preload.o
