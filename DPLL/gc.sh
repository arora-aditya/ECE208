for output in $(ls ../GraphCover/output_cnf/);
do
  if ./dpll.o ../GraphCover/output_cnf/$output | grep -Fxq 'UNSAT'; then
    echo 'got rekt:' $output
  fi;
done;
