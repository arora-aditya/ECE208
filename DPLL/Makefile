lso := $(shell ls ../GraphCover/output_cnf/)

WARNINGS = -Wall -Wextra
STD = -std=c++14
OUTPUT = dpll.o

# DEFAULT: help

release: ## release for submission with optimisations
	g++ dpll.cc -O3 $(STD) -o $(OUTPUT)
	@echo "\033[36mtry out: make help\033[0m"

build: ## build for local development
	g++ dpll.cc $(STD) $(WARNINGS) -o $(OUTPUT)

.PHONY: test
test: build ## test all files inside test/ directory
	./test.sh
.PHONY: test_one
test_one: build ## test test1.cnf file
	./$(OUTPUT) test/test1.cnf

.PHONY: clean
clean: ## remove output files
	rm *.o

.PHONY: help
help: ## probably the command you're looking for
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-10s\033[0m %s\n", $$1, $$2}'

check_gc: ## check SAT solver == SAT on all graph cover output files
	@sh gc.sh
	@echo "\033[36mIf you don't see any output above you're good\033[0m";
