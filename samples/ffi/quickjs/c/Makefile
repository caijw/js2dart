OBJDIR=.obj
CC=gcc
CFLAGS=-g -Wall -MMD -MF $(@F).d -Wno-array-bounds
LDFLAGS=-flto
CFLAGS_OPT=$(CFLAGS)
DEFINES:=-D_GNU_SOURCE
CFLAGS+=$(DEFINES)

LIBS=/usr/local/lib/quickjs/libquickjs.a
INCLUDES=-I/usr/local/include/quickjs

jscore.o: jscore.c
	$(CC) $(LDFLAGS) $(CFLAGS_OPT) -c $(INCLUDES) -o $@ jscore.c

jscore: jscore.o
	gcc -dynamiclib -undefined suppress -flat_namespace jscore.o -o ../jscore.dylib $(LIBS)

clean:
	rm ./jscore.o
	rm ./jscore.o.d
	rm ../jscore.dylib
