INCLUDE=`zeo-config -I`
LIB=`zeo-config -L`
LINK=`zeo-config -l`

CC=gcc
CFLAGS=-ansi -Wall -O3 $(LIB) $(INCLUDE) -funroll-loops

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean :
	rm -f *.o *~ core *test *.ztk ?
