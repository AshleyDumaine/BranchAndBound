SRCS=knapsack_serial.c knapsack.c priorityQueue.c heap.c

OFILES=knapsack_serial.c knapsack.c priorityQueue.c heap.c

CC=cc
LD=cc

CFLAGS=-c -g -pthread
LFLAGS=-g -pthread

all: knapsack_serial knapsack
	@echo "Done compiling!"

knapsack_serial: $(OFILES)	
	$(CC) $(LFLAGS) $(OFILES) -o $@

knapsack: $(OFILES)	
	$(CC) $(LFLAGS) $(OFILES) -o $@

priorityQueue.o : priorityQueue.c
	$(CC) $(CFLAGS) $< -o $@

knapsack_serial.o : knapsack_serial.c
	$(CC) $(CFLAGS) $< -o $@

knapsack.o : knapsack.c
	$(CC) $(CFLAGS) $< -o $@

heap.o : heap.c
	$(CC) $(CFLAGS) $< -o $@

