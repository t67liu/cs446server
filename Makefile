all:
	g++ Binderfor446.cpp -o binder 
	g++ ServerFor446.cpp -pthread -o server

clean: 
	rm binder
	rm server
