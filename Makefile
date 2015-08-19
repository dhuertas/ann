CC = g++
CFLAGS = -g -Wall -O3
SHELL = /bin/bash

INCLUDE = \
  -I./src

LIBS =

SOURCES = $(shell find ./src -name '*.cc')

OBJS = $(shell find ./src -name '*.cc' | sed 's/.cc/.o/g')

TARGET = ann

all: $(TARGET)

default: $(TARGET)

-include $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.cc
	gcc -c $(CFLAGS) $(INCLUDE) $*.cc -o $*.o
	gcc -MM $(CFLAGS) $(INCLUDE) $*.cc > $*.d

clean:
	@rm -f ./src/*.o ./src/*.d
	@rm -rf $(TARGET).dSYM
	@rm -f $(TARGET)
