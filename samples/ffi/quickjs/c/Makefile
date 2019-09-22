OBJDIR=.obj
CC=gcc
CFLAGS=-g -Wall -MMD -MF $(@F).d -Wno-array-bounds
LDFLAGS=-flto
CFLAGS_OPT=$(CFLAGS)
DEFINES:=-D_GNU_SOURCE
CFLAGS+=$(DEFINES)

LIBS=/usr/local/lib/quickjs/libquickjs.a
INCLUDES=-I/usr/local/include/quickjs

hello_world.o: hello_world.c
	$(CC) $(LDFLAGS) $(CFLAGS_OPT) -c $(INCLUDES) -o $@ hello_world.c

dylib: hello_world.o
	gcc -dynamiclib -undefined suppress -flat_namespace hello_world.o -o ../hello_world.dylib $(LIBS)

clean:
	rm ./hello_world.o
	rm ./hello_world.o.d
	rm ../hello_world.dylib
