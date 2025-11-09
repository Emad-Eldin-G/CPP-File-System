CXX     = g++

CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main

# These are the two executables to be produced
main: main.cpp FileSystem.o
	$(CXX) $(CXXFLAGS) main.cpp FileSystem.o -o main

FileSystem.o: FileSystem.cpp FileSystem.h
	$(CXX) $(CXXFLAGS) -c FileSystem.cpp -o FileSystem.o

deepclean:
	rm -f *~ *.o main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
