all:
	g++ Binderfor446.cpp -o binder 
	g++ ServerFor446.cc -pthread -o server

clean: 
	rm binder
	rm server
