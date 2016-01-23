SRCS=branch_and_bound_knapsack.c knapsack_serial.c priorityQueue.c

OFILES=branch_and_bound_knapsack.c priorityQueue.c heap.c
OFILES2=knapsack_serial.c priorityQueue.c heap.c
LD=cc
CC=cc

CFLAGS=-c -O3 -pthread
LFLAGS=-g -pthread

all: knapsack knapsack_serial cpluspluscode
	@echo "Done compiling!"

knapsack_serial: knapsack_serial.o	
	$(CC) $(LFLAGS) $(OFILES2) -o $@

knapsack: knapsack.o	
	$(CC) $(LFLAGS) $(OFILES) -o $@

priorityQueue.o : priorityQueue.c
	$(CC) $(CFLAGS) $< -o $@

knapsack_serial.o : knapsack_serial.c
	$(CC) $(CFLAGS) $< -o $@

knapsack.o : branch_and_bound_knapsack.c
	$(CC) $(CFLAGS) $< -o $@

heap.o : heap.c
	$(CC) $(CFLAGS) $< -o $@

cpluspluscode :
	g++ -pthread -O3 -o dp_knapsack dp_knapsack.cpp heap.c priorityQueue.c
	g++ -pthread -O3 -o serial_bnb knapsack_serial.cpp heap.c priorityQueue.c
	g++ -pthread -O3 -o speedy_knapsack branch_and_bound_knapsack.cpp heap.c priorityQueue.c

clean: 
	rm -f knapsack.o knapsack_serial.o dp_knapsack knapsack knapsack_serial serial_bnb speedy_knapsack
