CC = gcc
CFLAGS = -g -Wall -std=c99
INCLUDES =
LFLAGS = 
LIBS = -lsqlite3 -lreadline
OBJS = dict.o
TARGET = dict
DICTIONARY = data/dictdb.sqlite

INSTALL_DIR = /home/lusaisai/.dict

all: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

clean: 
	rm $(TARGET) $(OBJS)

install: $(TARGET) $(DICTIONARY)
	mkdir -p $(INSTALL_DIR)
	cp $(TARGET) $(INSTALL_DIR)
	cp $(DICTIONARY) $(INSTALL_DIR)
	@echo "To launch the dictionary, please run command: "$(INSTALL_DIR)/$(TARGET)

