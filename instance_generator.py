import random

with open("data/instance.txt", 'w') as f:
	f.write('1000000\n')
	for i in range(1, 1000000):
		f.write('{} {} {}\n'.format(i, random.randrange(1, 50000), random.randrange(1,10000)))
	f.write('{}'.format(random.randrange(100, 100000)))
