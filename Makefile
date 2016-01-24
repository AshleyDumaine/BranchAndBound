all: cpluspluscode
	@echo "Done compiling!"
cpluspluscode :
	g++ -pthread -O3 -o dp_knapsack dp_knapsack.cpp heap.c priorityQueue.c
	g++ -pthread -O3 -o serial_bnb knapsack_serial.cpp heap.c priorityQueue.c
	g++ -pthread -O3 -o speedy_knapsack branch_and_bound_knapsack.cpp heap.c priorityQueue.c

clean: 
	rm -f dp_knapsack serial_bnb speedy_knapsack
