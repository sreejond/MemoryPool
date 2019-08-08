CC=g++
CFLAGS=-std=c++11 -c -g -Wall

all: MemoryPoolTest1 MemoryPoolTest2 MemoryPoolTest3

MemoryPoolTest1: MemoryPoolTest1.o
	$(CC) MemoryPoolTest1.o -o MemoryPoolTest1 -lpthread

MemoryPoolTest2: MemoryPoolTest2.o
	$(CC) MemoryPoolTest2.o -o MemoryPoolTest2 -lpthread

MemoryPoolTest3: MemoryPoolTest3.o
	$(CC) MemoryPoolTest3.o -o MemoryPoolTest3 -lpthread

MemoryPoolTest1.o: MemoryPoolTest1.cpp
	$(CC) $(CFLAGS) MemoryPoolTest1.cpp
	
MemoryPoolTest2.o: MemoryPoolTest2.cpp
	$(CC) $(CFLAGS) MemoryPoolTest2.cpp

MemoryPoolTest3.o: MemoryPoolTest3.cpp
	$(CC) $(CFLAGS) MemoryPoolTest3.cpp

clean:
	rm -rf *.o MemoryPoolTest1 MemoryPoolTest2 MemoryPoolTest3
