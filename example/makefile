CC=gcc
CFLAGS=-ansi -Wall -O3 -funroll-loops -g `zeo-config --cflags`
LINK=`zeo-config -l`

CXX=g++
CXXFLAGS=-std=c++17 -Wall -O3 -funroll-loops -g `zeo-config --cflags`
LINKCXX=`zeo-config -lcpp`

TARGET=$(shell ls *.c | xargs -i basename {} .c | tr -s "\n" " ") $(shell ls *.cpp | xargs -i basename {} .cpp | tr -s "\n" " ")

all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LINKCXX)
clean :
	-@rm -f *.o *~ core *test *.ztk ?
