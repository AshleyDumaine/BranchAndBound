###Ashley Dumaine
###CSE5095 Spring 2016
###Discrete Optimization

Assignment 1
------------

##Introduction
Dynamic programming, the greedy algorithm, and branch and bound were explored
and compared in C++, Python, and C respectively to find a feasible solution to 
the knapsack problem. Python was chosen for the greedy solution since the 
quick linear algorithm makes Python’s slowness negligible. C++ and C were 
chosen for the other two methods since both are time-consuming as they explore 
potentially an extremely large amount of solutions and are non-linear 
algorithms. Based on the problem instance, each of these algorithms have 
varying abilities of computing the solution that’s either acceptable in the 
case of greedy knapsack or within a reasonable amount of time in the case of 
dynamic programming and branch and bound.

##Dynamic Programming Approach
	As expected, dynamic programming works well only on very small problem 
instances. For the sample instance of 32, it didn’t return after over 5 
minutes, but it did complete in 4.572611 seconds and return the correct optimal 
value of 604 for the following test instance:
20
	1	19 	1
	2	39	52
	3 	5	63
	4	70	86
	5	19	57
	6	14 	2
	7	90	82
	8	33	38
	9	80	31
  	10	98	63
   	11	11 	5
   	12	10	25
   	13	55	28
   	14	88	96
   	15	15	39
   	16	49	56
   	17	86	41
   	18	63	15
   	19	63	73
   	20	19	99
370

##Greedy Approach
Due to the nature of the greedy algorithm and the fact that the problem is 0/1 
knapsack instead of fractional, the greedy solution returns very quickly, but 
is not always optimal. For the same instance above, the greedy solution 
actually returned the optimal value of 604 in only 0.002304 seconds. It can 
also run for large instances of 2,000 elements like in 
CSE5095/data/BIGINPUT3.txt in only 0.022766 seconds although it’s returns an 
answer of 52339786 rather than the optimal of 52360767. Overall, greedy is very 
fast but has the potential of returning a less than optimal solution.

##Branch and Bound Approach
The branch and bound method is a quicker way to get the global optimal solution 
in the case of a large number of elements to chose from. Even the serial 
version of branch and bound is significantly faster than any other algorithm 
besides greedy since greedy is the first step in branch and bound for 
determining the order in which to insert elements into the heap. A 
multithreaded version of branch and bound was also implemented, however, the 
thread overhead is too high for only a quad- or dual-core machine, so the 
serial implementation of branch and bound is determined to be the best 
algorithm to get the right answer and quickly.

##Speedup
Supposedly C++'s std::sort is over twice as fast at sorting a million elements 
compared to C's qsort function. Since sorting is needed for determining the 
upper bound, std::sort was used instead. For instances with a large number of 
items to choose from, there was a significant speedup in the overall runtime. 
The old C code from CSE3100 was cleaned up and further condensed to decrease 
instruction count and the fields of the structs were rearranged for optimal 
packing. For another boost, the code was compiled with -O3 which made it run 
at least twice as fast as if it had not been compiled with optimization turned 
on. 

Below is the old CSE3100 C vs new C++ version runtume for the serial 
implementation of branch and bound and the multithreaded version with only 2 
threads, respectively:

x@ubuntu:~/Desktop/CSE5095$ ./knapsack\_serial data/t100.txt
lower bound is: 3208
Elapsed: 4.747830 seconds
x@ubuntu:~/Desktop/CSE5095$ ./serial\_bnb data/t100.txt
lower bound is: 3208
Elapsed: 2.076265 seconds

x@ubuntu:~/Desktop/CSE5095$ ./knapsack data/t100.txt 2
optimal value: 3208
optimality: 1
time to solve: 14.123101 seconds
x@ubuntu:~/Desktop/CSE5095$ ./speedy\_knapsack data/t100.txt 2
lower bound is: 3208
Elapsed: 7.437743 seconds

One last-minute realization was that the runtime and number of branch and 
bound nodes was related to the initial heap size. The old code from CSE3100 
made the heap 10 times too large in order to avoid possibly having to resize 
the heap. This is actually detrimental to the runtime, as changing the initial 
heap size to only the length of the items to choose from still yields the 
correct answer as much as 30 times faster using tougher100.txt as the 
benchmark:

ashley@ubuntu:~/Desktop/BranchAndBound-master$ ./serial\_bnb data/tougher100.txt
Optimal value: 3148 
Optimality: 1
2.837460 seconds 
Nodes traversed: 23980068 

ashley@ubuntu:~/Desktop/BranchAndBound-master$ ./serial\_bnb data/tougher100.txt
Optimal value: 3148 
Optimality: 1
0.107571 seconds 
Nodes traversed: 803316 

Although less nodes are traversed, the answer is still correct, confirmed on 
several other knapsack instances.

##Other Remarks
To test the final version of branch and bound, a trivial Python instance 
generator was written. It was observed in a sample branch and bound test case 
that 100 elements where the weights ranged from [1,100] and the profit for 
each item was 10 more than the value of the weight, the problem generated a 
large number of nodes. After running the instance generator several times and 
solving the resulting test case, one instance was found that generated nearly 
24 million branch and bound nodes. Oddly, this test case was quicker to solve 
than some other instances generated by the Python instance generator that 
resulted in less nodes. However, as noted above, fixing the heap size 
significantly reduced the node count.
