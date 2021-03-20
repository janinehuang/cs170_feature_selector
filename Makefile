CXX = g++
main: main.cpp row.o fselector.o
	$(CXX) main.cpp row.o fselector.o

row.o: row.cpp row.h
	$(CXX) -c row.cpp

fselector.o: fselector.cpp fselector.h
	$(CXX) -c fselector.cpp
