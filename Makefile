.SUFFIXES : .c .o 
CFLAGS = -g -O0 -Wall -I./
CC = gcc
OBJS = main.o clog.o db.o
SRCS = $(OBJS:.o=.c)

clog: $(OBJS)
	$(CC) -o clog.cgi $(OBJS) -lsqlite3 -lfcgi

dep:
	gccmakedep $(SRCS)

clean:
	rm -f $(OBJS) clog core *~

main.o: main.c clog.h
clog.o: clog.c db.h
db.o: db.c


