CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp satnet.o
	$(CXX) $(CXXFLAGS) satnet.o mytest.cpp -o mytest

satnet.o: satnet.cpp satnet.h
	$(CXX) $(CXXFLAGS) -c satnet.cpp

run:
	./mytest

val:
	valgrind ./mytest