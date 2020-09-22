Steps to execute:
In the source code folder run the following commands:
[NOTE: To run the read command first execute the write command to generate the file and then use the read command]
1. make
2. ./MyDiskBench 16MB 2 WS
   pass three arguments to the executable
   argument 1 : record size (4KB/64KB/1MB/16MB) or (4kb/64kb/1mb/16mb)
   argument 2 : no. of threads (1/2/4/8/12/24/48)
   argument 3 : access pattern (WS/RS/WR/RR) or (ws/rs/wr/rr)
If required
3. make clean : to remove executable file,object files and text files.

Example : to perform read operation
1. Execute write operation to generate files
    ./MyDiskBench 16MB 2 WS
2. Execute the read operation on the generated file
    ./MyDiskBench 16MB 2 RS

The directory contains the following files:

1. Makefile : To compile the code and generate executable file named MyDiskBench
2. MyDiskBench.cpp : This file accepts input from the user and calls the respective 
                     function to execute the commands.
3. randomRead.cpp: File contains function to perform random read operation.
4. randomRead.h :  Header file for randomRead.cpp
5. randomWrite.cpp : File contains function to perform random write operation.
6. randomWrite.h :  Header file for randomWrite.cpp
7. sequentialRead.cpp: File contains function to perform sequential read operation.
8. sequentialRead.h : Header file for sequentialRead.cpp
9. sequentialWrite.cpp : File contains function to perform sequential write operation.
10. sequentialWrite.h : Header file for sequentialWrite.cpp
11. utils.cpp : File contains 2 functions. First to return random value (for random read/write)
                 and second to return filesize in bytes. 
12. utils.h : Header file for utils.cpp
13. execute_benchmark.sh : Script to execute random and sequential read/write for various access patterns,
                          thread count and record size.                 
14. benchmark_4KB.sh : Script to execute random read/write on 4KB record size with different thread count.