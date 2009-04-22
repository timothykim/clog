.SUFFIXES : .c .o 
CFLAGS = -g -O0
CC = gcc
OBJS = main.o clog.o 
SRCS = $(OBJS:.o=.c)

clog: $(OBJS)
	$(CC) -o clog.cgi $(OBJS) -lsqlite3 -lfcgi

dep:
	gccmakedep $(SRCS)

clean:
	rm $(OBJS) clog core

main.o: main.c clog.h
clog.o: clog.c 
