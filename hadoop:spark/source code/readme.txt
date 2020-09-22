This directory has the following files

1. Makefile: To compile the source code. Creates executable file MySort
2. MySort.cpp : Sorts the given dataset file using in-memory/external merge sort and stores the sorted result in text file.
3. HadoopSort.java: Sort given dataset using Hadoop.
4. SparkSort.java: Sort the given data using Spark.
5. hadoop-installation.sh: installation script for hadoop on namenode. This adds configuration for only haddop-env.sh and /etc/profile.d/hadoop.sh
6. spark-installation.sh: installation script for apache spark.
  
To run MySort.cpp:	

Steps to run:
1. make
2. ./MySort no_of_threads filename
    eg ./MySort 24 record_1GB.txt
    
Steps to clear executable and output files
1. make clean


To run Hadoop Sort

1. Compile the code
	 bin/hadoop com.sun.tools.javac.Main HadoopSort.java
2. Create jar
	jar cf hs.jar HadoopSort.class
3.  Execute code
	bin/hadoop jar hs.jar HadoopSort /home/input/ /home/output
4. Collect the output file 
	bin/hadoop fs -getmerge /home/out sort.txt


To run spark sort

1. Compile
	javac -cp jars/spark-core_2.12-3.0.0-preview2.jar:jars/scala-library-2.12.10.jar  SparkSort.java
	
2. Create jar
	jar cvf SparkSort.jar SparkSort.class
	
3.   Execute code  : It takes 3 additional argument
		- namenode ip with port (eg.  hdfs://192.168.122.69:9000)
		- input file path (eg. /home/input/data-1GB.txt )
		- output file path (eg. /home/output)
		
	time bin/spark-submit  --class SparkSort --master yarn --deploy-mode cluster --driver-memory 4g --executor-memory 2g --executor-cores 1 SparkSort.jar hdfs://192.168.122.69:9000 /home/input/data-1GB.txt /home/output_1GB
	
	
