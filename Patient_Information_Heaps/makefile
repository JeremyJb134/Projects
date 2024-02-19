CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp pqueue.o
	$(CXX) $(CXXFLAGS) pqueue.o mytest.cpp -o mytest

pqueue.o: pqueue.cpp pqueue.h
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

run:
	./mytest

val:
	valgrind ./mytest