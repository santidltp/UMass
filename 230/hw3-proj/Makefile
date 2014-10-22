CC = gcc
CFLAGS = --std=c99 -Wall -g

all: wordfreq wordfreq-fast

wordfreq: wset.o words.o wordfreq.o
	$(CC) $(CFLAGS) wset.o words.o wordfreq.o -o wordfreq

wordfreq-fast: wset.o words.o wordfreq-fast.o
	$(CC) $(CFLAGS) wset.o words.o wordfreq-fast.o -o wordfreq-fast

wset.o: wset.c wset.h
	$(CC) $(CFLAGS) -c wset.c

words.o: words.c words.h
	$(CC) $(CFLAGS) -c words.c

wordfreq.o: wordfreq.c words.h wset.h
	$(CC) $(CFLAGS) -c wordfreq.c

wordfreq-fast.o: wordfreq-fast.c words.h wset.h
	$(CC) $(CFLAGS) -c wordfreq-fast.c

test: buildtest
	./test/public-test

buildtest: all test/public-test.c
	make -C test

clean:
	rm -f *.o
	rm -f wordfreq wordfreq-fast test/public-test
	make -C test clean
