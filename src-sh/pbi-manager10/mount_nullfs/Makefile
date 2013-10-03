all: nullfs

nullfs: mount_nullfs.c
	${CC} -lutil -o mount_nullfs mount_nullfs.c getmntopts.c

install: nullfs
	install -o root -g wheel -m 755 mount_nullfs /usr/pbi/.mount_nullfs

clean:
	rm mount_nullfs
