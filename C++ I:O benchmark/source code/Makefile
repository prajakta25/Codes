all: MyDiskBench

MyDiskBench: MyDiskBench.o randomRead.o randomWrite.o sequentialRead.o sequentialWrite.o utils.o
	g++ -Wall  -pthread MyDiskBench.o randomRead.o randomWrite.o sequentialWrite.o sequentialRead.o utils.o -o MyDiskBench

MyDiskBench.o : MyDiskBench.cpp
	g++ -Wall  -c MyDiskBench.cpp

randomRead.o : randomRead.cpp
	g++ -Wall  -std=c++11 -std=c++11 -c randomRead.cpp

randomWrite.o : randomWrite.cpp
	g++ -Wall -std=c++11 -std=c++11 -c randomWrite.cpp

sequentialWrite.o : sequentialWrite.cpp
	g++ -Wall -std=c++11 -c sequentialWrite.cpp

sequentialRead.o : sequentialRead.cpp
	g++ -Wall -std=c++11 -c sequentialRead.cpp

utils.o : utils.cpp
	g++ -Wall -std=c++11 -c utils.cpp

clean:
	rm -rf *o *txt MyDiskBench