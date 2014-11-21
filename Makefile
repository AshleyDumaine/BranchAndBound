SRCS=knapsack.c priorityQueue.c 

OFILES=knapsack.c priorityQueue.c

CC=cc
LD=cc

CFLAGS=-c -g -pthread
LFLAGS=-g -pthread

all: knapsack
	@echo "Done compiling!"

knapsack: $(OFILES)	
	$(CC) $(LFLAGS) $(OFILES) -o $@

priorityQueue.o : priorityQueue.c
	$(CC) $(CFLAGS) $< -o $@

knapsack.o : knapsack.c
	$(CC) $(CFLAGS) $< -o $@


