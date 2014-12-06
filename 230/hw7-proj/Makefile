CC = gcc
LIBS = -lpthread  -lrt
INCS = -I./
CCFLAGS = -std=gnu99 -O2

all: client
client: csapp.o client.c
	$(CC) $(CCFLAGS) $^ -o $@  $(LIBS) $(INCS)
csapp.o: csapp.c csapp.h
	$(CC) $(CCFLAGS) -c $< -o $@  $(INCS)

clean:
		rm -f *.o client csapp