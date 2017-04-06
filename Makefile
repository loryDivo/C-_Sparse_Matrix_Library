main.exe : main.o
	g++ -static-libgcc -static-libstdc++ main.o -o main.exe
	
main.o : main.cpp SparseMatrix.h
	g++ -c main.cpp -o main.o