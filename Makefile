.SUFFIXES : .c .o 
CFLAGS = -g -O0 -Wall -I./
CC = gcc
OBJS = main.o clog.o hash.o db.o
TEST_OBJS = test.o hash.o
DEPLOY_OBJS = *.ct clog.db clog.cgi
SRCS = $(OBJS:.o=.c)

clog: $(OBJS)
	$(CC) -o clog.cgi $(OBJS) -lsqlite3 -lfcgi

test: $(TEST_OBJS)
	$(CC) -o test $(TEST_OBJS) -lsqlite3 -lfcgi

dep:
	gccmakedep $(SRCS)

clean:
	rm -f $(OBJS) clog core *~

deploy:
	cp -t /Library/WebServer/CGI-Executables $(DEPLOY_OBJS)

test.o: test.c
main.o: main.c clog.h
clog.o: clog.c db.h
hash.o: hash.c
db.o: db.c

