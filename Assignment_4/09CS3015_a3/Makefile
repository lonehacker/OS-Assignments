all: controller msgclient

controller: controller.c
	cc -Wall controller.c -o controller

msgclient: libmsg.c msgclient.c
	cc -Wall -c libmsg.c
	ar -rcs libmsg.a libmsg.c
	cc -Wall -c msgclient.c
	cc msgclient.o -L. -lmsg -o myclient

