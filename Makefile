TARGET = factorial.exe
SRC = main.c factorial.c
OBJ = main.o factorial.o
ASM = factorial.s
CC = gcc
CFLAGS = -Wall -O2 -W

all: $(TARGET)

# сборка исполняемого файла из объектных
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# .c в .o
main.o: main.c factorial.h
	$(CC) $(CFLAGS) -c main.c

factorial.o: factorial.c factorial.h
	$(CC) $(CFLAGS) -c factorial.c

# ассемблер из factorial.c
asm: factorial.c
	$(CC) $(CFLAGS) -S factorial.c -o $(ASM)

# Очистка
clean:
	del *.o *.exe *.s