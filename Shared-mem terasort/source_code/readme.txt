Directory contains 10 files:

1. Makefile: To compile the source code. Creates executable file MySort
2. MySort.cpp : Sorts the given dataset file using in-memory/external merge sort and stores the sorted result in text file.
3. Log files : contains 8 log files for MySort and Linux Sort.
   They contain the following information:
   - Data Read
   - Data Write
   - Sort Time
   - Throughput
   - valsort output
  
	

Steps to run:
1. make
2. ./MySort no_of_threads filename
    eg ./MySort 24 record_1GB.txt
    
Steps to clear executable and output files
1. make clean
