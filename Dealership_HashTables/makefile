CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp dealer.o
	$(CXX) $(CXXFLAGS) dealer.o mytest.cpp -o mytest

dealer.o: dealer.cpp dealer.h
	$(CXX) $(CXXFLAGS) -c dealer.cpp

run:
	./mytest

val:
	valgrind ./mytest