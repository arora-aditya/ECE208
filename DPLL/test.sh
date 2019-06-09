#!/bin/bash

for filename in test/*.cnf
	do
	test_out="$(./dpll.o $filename)"
	echo "Testing $filename ...  $test_out"
done
