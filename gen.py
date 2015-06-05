#/usr/bin/python

import random
import sys

def main(rows, cols, times):
	for i in range(times):
		a = range(0, rows*cols)
		random.shuffle(a)
		print rows, cols, str(a).strip('[]').replace(',', ' ')

if __name__ == '__main__':
	main(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))


