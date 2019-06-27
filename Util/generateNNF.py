from random import *
import sys
#ext = randint(0,1111) # if you want this to be safe use something else
ext = ""

if __name__ == "__main__":
	if len(sys.argv) > 1:
		count = int(sys.argv[1])
	else:
		count = 1
	if len(sys.argv) > 2:
		start = int(sys.argv[2])
	else:
		start = 0
	while(count):
		file_name = "test_case" + str(start+count) + ".nnf"
		fp = open(file_name, "w+")
		clauseLength = randint(5,100)
		while (clauseLength > 0):
			var = randint(-50,50)
			while (var == 0) :
				var = randint(-15,15)
			fp.write(str(var))
			clauseLength = clauseLength-1
			if (clauseLength > 0):
				operator = randint(0,1)
				if (operator == 0) :
					fp.write(".")
				else :
					fp.write("+")
		fp.write(' 0')
		count -= 1
