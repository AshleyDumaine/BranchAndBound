SRCS=knapsack_serial.c priorityQueue.c 

OFILES=knapsack_serial.c priorityQueue.c

CC=cc
LD=cc

CFLAGS=-c -g -pthread
LFLAGS=-g -pthread

all: knap_serial
	@echo "Done compiling Knapsack_serial!"

knap_serial: $(OFILES)	
	$(CC) $(LFLAGS) $(OFILES) -o $@

priorityQueue.o : priorityQueue.c
	$(CC) $(CFLAGS) $< -o $@

knapsack_serial.o : knapsack_serial.c
	$(CC) $(CFLAGS) $< -o $@


