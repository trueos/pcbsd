
all: pbi_preload

pbi_preload: pbi_preload.c
	${CC} -fPIC -c -o pbi_preload.o pbi_preload.c
	${CC} -shared -o pbi_preload.so pbi_preload.o 
	${CC} -mtune=i386 -march=i386 -m32 -fPIC -c -o pbi_preload32.o pbi_preload.c
	${CC} -mtune=i386 -march=i386 -m32 -shared -o pbi_preload32.so pbi_preload32.o

install: pbi_preload
	install -o root -g wheel -m 755 pbi_preload.so /usr/pbi/.pbi_preload.so
	install -o root -g wheel -m 755 pbi_preload32.so /usr/pbi/.pbi_preload32.so

clean:
	rm pbi_preload.so
	rm pbi_preload.o
	rm pbi_preload32.so
	rm pbi_preload32.o
