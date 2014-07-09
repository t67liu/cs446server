all:
	g++ Binderfor446.cpp -o binder 
	./compile
	./compile2
clean: 
	rm binder
