
CC=g++
CXXFLAGS=-Wall -I ./ -g #-DFORWARD_CHECKING
AR=ar

TARGET=gse.app
SOURCES=$(wildcard *.cc)
OBJ=$(SOURCES:.cc=.o)
GTEST_DIR=gtest-1.7.0

$(TARGET):$(OBJ) 
	$(CC) -o $@ -std=c++11 $^ $(CXXFLAGS) $(LIBS)
	$(AR) -rv libgse.a $(OBJ)

.cc.o:
	$(CC) $(CXXFLAGS) -std=c++11 -c $< -o $@ 

.PHONY: clean

clean:
	rm -rf *.o *~ $(OBJ) $(TARGET) *.a
