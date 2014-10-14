CC = gcc
CFLAGS = -I/usr/include --std=c99 -Wall -g
LDFLAGS = -L/usr/lib/i386-linux-gnu -lrt -lm -lpthread
LDTESTFLAGS = -lcheck $(LDFLAGS)
OBJS = ../wset.o ../words.o

all: public-test private-test

public-test: public-test.o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) public-test.o -o public-test $(LDTESTFLAGS)

public-test.o: public-test.c 
	$(CC) $(CFLAGS) -c public-test.c

private-test: private-test.o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) private-test.o -o private-test $(LDTESTFLAGS)

private-test.o: private-test.c 
	$(CC) $(CFLAGS) -c private-test.c

clean:
	rm -f *.o
	rm -f public-test
	rm -f private-test
