#!/bin/sh

test_out=$(./dpll.o cnf_first.txt)
if [ "$test_out" = "SAT" ]; then
	echo "Test successful"
else
	echo "Error: Test Output:"
	echo $test_out
fi
