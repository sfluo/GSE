
CC=g++
GTEST_DIR=./gtest-1.7.0
SOURCES=$(wildcard *.cc)
OBJ=$(SOURCES:.cc=.o) 
TARGET=gsetest.app

CXXFLAGS=-I./ -I../ -I$(GTEST_DIR)/include -g
LDFLAGS=-g -L$(GTEST_DIR)/lib -L./ -L../ -lgse -lgtest -lpthread

GTEST_ALL_CC=$(GTEST_DIR)/src/gtest-all.cc

all: $(TARGET)

$(TARGET):$(OBJ) gtest-all.o
	$(CC) $(LDFLAGS) $^ -o $@
	$(AR) -rv libgtest.a gtest-all.o

gtest-all.o:
	$(CC) -I. -I$(GTEST_DIR) -I$(GTEST_DIR)/include -c $(GTEST_ALL_CC)

.cc.o:
	$(CC) $(CXXFLAGS) -c $< -o $@ -std=c++0x

.PHONY: clean

clean:
	rm -rf *.o *~ $(OBJ) $(TARGET)
