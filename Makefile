OBJECTS = main.o clog.o
CC = gcc
TARGET = clog

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) -lsqlite3

main.o : clog.h main.c
clog.o : clog.h clog.c

clean :
	rm $(OBJECTS) $(TARGET) core
