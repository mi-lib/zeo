CFLAGS=`zeo-config --cflags`
LINK=`zeo-config -l`

TARGET=$(shell ls *.c | xargs -i basename {} .c | tr -s "\n" " ")

CC=gcc
CFLAGS+=-ansi -Wall -O3 -funroll-loops -std=c99 -g

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean :
	-@rm -f *.o *~ core *test *.ztk ?
