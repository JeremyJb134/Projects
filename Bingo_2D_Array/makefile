CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp bingo.o 
	$(CXX) $(CXXFLAGS) bingo.o mytest.cpp -o mytest

bingo.o: bingo.cpp bingo.h
	$(CXX) $(CXXFLAGS) -c bingo.cpp

val:
	valgrind ./mytest

run:
	./mytest