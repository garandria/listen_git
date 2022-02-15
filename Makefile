.PHONY: all

CC = gcc
CFLAGS = -g
LFLAGS = -lgit2 -lz
APPS = lg_main

all: $(APPS)

% : %.c
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	$(RM) $(APPS)
	$(RM) -r *.dSYM
