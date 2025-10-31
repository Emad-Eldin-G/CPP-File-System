CXX     = g++

CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: main FileSystemTesterMain

# These are the two executables to be produced
main: main.cpp FileSystem.o
	$(CXX) $(CXXFLAGS) main.cpp FileSystem.o -o main

FileSystemTesterMain: FileSystemTesterMain.cpp FileSystem.o FileSystemTester.o
	$(CXX) $(CXXFLAGS) FileSystemTesterMain.cpp FileSystem.o FileSystemTester.o -o FileSystemTesterMain

FileSystem.o: FileSystem.cpp FileSystem.h
	$(CXX) $(CXXFLAGS) -c FileSystem.cpp -o FileSystem.o

FileSystemTester.o: FileSystemTester.cpp FileSystemTester.h FileSystem.h
	$(CXX) $(CXXFLAGS) -c FileSystemTester.cpp -o FileSystemTester.o

deepclean:
	rm -f *~ *.o FileSystemTesterMain main main.exe *.stackdump

clean:
	rm -f *~ *.o *.stackdump
