.PHONY: dev run

dev:
	g++ -Wall -Werror -pedantic -g -std=c++23 bfjit.cpp -o bfjit

run:
	g++ -Wall -Werror -pedantic -g -std=c++23 -O3 bfjit.cpp -o bfjit
