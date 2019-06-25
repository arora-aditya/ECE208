from random import * 


	
#ext = randint(0,1111) # if you want this to be safe use something else
ext = ""
file_name = "test_case" + str(ext) + ".nnf" 

if __name__ == "__main__": 
	fp = open(file_name, "w+")
	clauseLength = randint(5,10)
	while (clauseLength > 0): 
		var = randint(-10,10)
		while (var == 0) :
			var = randint(-10,10)
		fp.write(str(var))
		clauseLength = clauseLength-1
		if (clauseLength > 0): 
			operator = randint(0,1)
			if (operator == 0) : 
				fp.write(".")
			else : 
				fp.write("+")	
