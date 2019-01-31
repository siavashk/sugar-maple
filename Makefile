CXX=g++
CXXFLAGS=-std=c++11

connectedcomponent:
	$(CXX) $(CXXFLAGS) connectedcomponent.cpp -o connectedcomponent.o

av:
	$(CXX) $(CXXFLAGS) articulationvertices.cpp -o articulationvertices.o

all: connectedcomponent av

clean:
	rm *o
