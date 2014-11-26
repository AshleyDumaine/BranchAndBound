SRCS=knapsack.c knapsack_serial.c priorityQueue.c heap.c

OFILES=knapsack.c priorityQueue.c heap.c
OFILES2=knapsack_serial.c priorityQueue.c heap.c
LD=cc
CC=cc

CFLAGS=-c -g -pthread
LFLAGS=-g -pthread

all: knapsack knapsack_serial
	@echo "Done compiling!"

knapsack_serial: knapsack_serial.o	
	$(CC) $(LFLAGS) $(OFILES2) -o $@

knapsack: knapsack.o	
	$(CC) $(LFLAGS) $(OFILES) -o $@

priorityQueue.o : priorityQueue.c
	$(CC) $(CFLAGS) $< -o $@

knapsack_serial.o : knapsack_serial.c
	$(CC) $(CFLAGS) $< -o $@

knapsack.o : knapsack.c
	$(CC) $(CFLAGS) $< -o $@

heap.o : heap.c
	$(CC) $(CFLAGS) $< -o $@

