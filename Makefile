CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++11


Projet: main.o Mapper.o
	$(CXX) -o my_project main.o Mapper.o
	

Mapper.o :
	g++ $(CXXFLAGS) -c Mapper.cpp
main.o:
	g++ $(CXXFLAGS) -c main.cpp

clean:
	rm *.o
	rm my_project

