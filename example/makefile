CFLAGS=`zeo-config --cflags`
LINK=`zeo-config -l`

CC=gcc
CFLAGS+=-ansi -Wall -O3 -funroll-loops

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean :
	-@rm -f *.o *~ core *test *.ztk ?
