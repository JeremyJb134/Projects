CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp csr.o 
	$(CXX) $(CXXFLAGS) csr.o mytest.cpp -o mytest

csr.o: csr.cpp csr.h
	$(CXX) $(CXXFLAGS) -c csr.cpp

val:
	valgrind ./mytest

run:
	./mytest