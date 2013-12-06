CC = gcc
CFLAGS = -g -Wall -std=c99
INCLUDES =
LFLAGS = 
LIBS = -lsqlite3
OBJS = dictdb_create.o
TARGET = dictdb_create

all: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

clean: 
	rm $(TARGET) $(OBJS)
