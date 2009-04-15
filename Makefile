.SUFFIXES : .c .o 
CFLAGS = -g
CC = gcc
OBJS = main.o clog.o 
SRCS = $(OBJS:.o=.c)

clog: $(OBJS)
	$(CC) -o clog $(OBJS) -lsqlite3

dep:
	gccmakedep $(SRCS)

clean:
	rm $(OBJS) clog core

# DO NOT DELETE
main.o: main.c clog.h /usr/include/sqlite3.h \
  /usr/lib/gcc/i686-apple-darwin9/4.0.1/include/stdarg.h
clog.o: clog.c /usr/include/sqlite3.h \
  /usr/lib/gcc/i686-apple-darwin9/4.0.1/include/stdarg.h \
  /usr/include/string.h /usr/include/_types.h /usr/include/sys/_types.h \
  /usr/include/sys/cdefs.h /usr/include/machine/_types.h \
  /usr/include/i386/_types.h /usr/include/stdlib.h \
  /usr/include/available.h /usr/include/sys/wait.h \
  /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h \
  /usr/include/machine/signal.h /usr/include/i386/signal.h \
  /usr/include/i386/_structs.h /usr/include/sys/_structs.h \
  /usr/include/machine/_structs.h /usr/include/mach/i386/_structs.h \
  /usr/include/sys/resource.h /usr/include/machine/endian.h \
  /usr/include/i386/endian.h /usr/include/sys/_endian.h \
  /usr/include/libkern/_OSByteOrder.h \
  /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h \
  /usr/include/machine/types.h /usr/include/i386/types.h \
  /usr/include/stdio.h /usr/include/time.h /usr/include/_structs.h clog.h
