# makefile - compile kbdlogd.

BIN_GCC=gcc

kbdlogd : src/kbdlogd.o
	${BIN_GCC} -pthread -o kbdlogd src/kbdlogd.o

kbdlogd.o : src/kbdlogd.c src/kbdlogd.h
	${BIN_GCC} -c src/kbdlogd.c -o src/kbdlogd.o

clean :
	rm -fr kbdlogd src/*.o

install:
	install -o root -g root -m755 kbdlogd /usr/sbin
	install -o root -g root -m644 man/kbdlogd.8.gz /usr/man/man8

uninstall:
	rm -fr /usr/sbin/kbdlogd
	rm -fr /usr/man/man8/kbdlogd.8.gz
