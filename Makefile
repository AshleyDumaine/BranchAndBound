all: cpluspluscode
	@echo "Done compiling!"
cpluspluscode :
	@g++ -pthread -O3 -o dp_knapsack dp_knapsack.cpp heap.c
	@g++ -pthread -O3 -o serial_bnb knapsack_serial.cpp heap.c
	@g++ -pthread -O3 -o multithreaded_bnb branch_and_bound_knapsack.cpp heap.c

clean: 
	rm -f dp_knapsack serial_bnb multithreaded_bnb
