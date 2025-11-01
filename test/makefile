ZEO_CFLAGS=`zeo-config --cflags`
LINK=`zeo-config -l`
LINKCXX=`zeo-config -lcpp`

CC=gcc
CFLAGS=-ansi -Wall -O3 -funroll-loops ${ZEO_CFLAGS}

CXX=g++
CXXFLAGS=-std=c++17 -Wall -O3 -funroll-loops ${ZEO_CFLAGS}

%: %.c
	@$(CC) $(CFLAGS) -o $@ $< $(LINK)
%: %.cpp
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LINKCXX)
clean :
	-@rm -f *.o *~ core *test
