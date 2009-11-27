.SUFFIXES : .c .o 
CFLAGS = -g -O0 -Wall -I./
CC = gcc
OBJS = main.o clog.o hash.o db.o atom.o
TEST_OBJS = test.o hash.o atom.o
DEPLOY_OBJS = *.ct clog.db clog.cgi
SRCS = $(OBJS:.o=.c)
CFLAGS = `xml2-config --cflags`
LIBS = `xml2-config --libs`

clog: $(OBJS)
	$(CC) -o clog.cgi $(OBJS) -lsqlite3 -lfcgi $(LIBS)

test: $(TEST_OBJS)
	$(CC) -o test $(TEST_OBJS) -lsqlite3 -lfcgi

dep:
	gccmakedep $(SRCS)

clean:
	rm -f $(OBJS) clog core *~

deploy:
	cp --preserve=mode -t /Library/WebServer/CGI-Executables $(DEPLOY_OBJS)

test.o: test.c
main.o: main.c clog.h
clog.o: clog.c db.h
hash.o: hash.c
db.o: db.c

