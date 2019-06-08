#!/bin/bash

for filename in test/*.cnf
	do
	test_out="$(./dpll.o $filename)"
	echo "Testing $filename ...  $test_out"
done



#test_out=$(./dpll.o cnf_first.txt)
#if [ "$test_out" = "SAT" ]; then
#	echo "Test successful"
#else
#	echo "Error: Test Output:"
#	echo $test_out
#fi
