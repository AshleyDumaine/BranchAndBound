import time
import sys
start = time.time()
# read input
num_items = 0
input_list = []
cap = 0
curr_weight = 0
contents = []
profit = 0

with open(str(sys.argv[1])) as f:
	num_items = int(f.readline().strip())
	for i in range(num_items):
		input_list.append(list(map(int, f.readline().split())))
	cap = int(f.readline().strip())
	# sort by value-to-weight ratios (greedy)
	input_list.sort(key=lambda x: float(x[1]) / float(x[2]), reverse=True)
	
	idx = 0
	item = input_list[idx]
	while(curr_weight + item[2] < cap):
		if item not in contents:
			profit += item[1]
			contents.append(item)
			curr_weight += item[2]
		idx += 1
		if (idx == len(input_list)):
			break
		item = input_list[idx]
	print('optimal value: {}\noptimality: 0'.format(profit))
	print('time to solve: {} seconds'.format(time.time() - start))
