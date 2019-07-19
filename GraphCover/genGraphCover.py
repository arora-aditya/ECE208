import sys

if __name__ == main:
	if len(sys.argv != 2):
		print("Usage : genGraphCover _size_")
		return 
	else:
		size = sys.argv(1)
		print("Creating Adjacency Matrix of Size ", size)
		f = open("testcase.cov","w")
		
