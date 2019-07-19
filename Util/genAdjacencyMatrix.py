import sys
import random

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print(f"Usage : {sys.argv[0]} _size_")
	else:
		size = int(sys.argv[1])
		print(f"creating adjacency matrices of Size {size}")
		total_count = 100
		for count in range(total_count):
			fp = open(f"adjacency_matrices/testcase_{count}.cov","w")
			for i in range(size):
				for j in range(i):
					fp.write(str(random.randint(0,1)))
				for j in range(i, size):
					fp.write(str(0))
				fp.write("\n")
