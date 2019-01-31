CXX=g++
CXXFLAGS=-std=c++11

scc:
	$(CXX) $(CXXFLAGS) scc.cpp -o scc.o

all: scc

clean:
	rm *o
