all:
	g++ Binderfor446.cpp -lpthread -o binder 
	$$ ./compile
	$$ ./compile2
clean: 
	rm binder
