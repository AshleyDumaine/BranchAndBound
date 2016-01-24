import random
# some generated case cause millions of nodes to be traversed
# to find a solution
with open("data/tough.txt", 'w') as f:
	f.write('101\n')
	for i in range(1, 101):
		num = random.randrange(1, 101)
		f.write('{} {} {}\n'.format(i, num + 10, num))
	f.write('2508')
