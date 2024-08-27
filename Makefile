.PHONY: compile

compile:
	g++ -Wall -Werror -pedantic -g -std=c++23 bfjit.cpp -o bfjit
	g++ -Wall -Werror -pedantic -g -std=c++23 runbin.cpp -o runbin
