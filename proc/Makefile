CC = gcc
CFLAGS = -Wall -Wextra -g

all: parent.exe child.exe

parent.exe: main.o
	$(CC) $(CFLAGS) -o parent.exe main.o -lws2_32

child.exe: child.o
	$(CC) $(CFLAGS) -o child.exe child.o -lws2_32

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

child.o: child.c
	$(CC) $(CFLAGS) -c child.c

clean:
	rm -f *.o parent.exe child.exe