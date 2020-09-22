#!/bin/bash
declare -a workload=(WR RR)
declare -a concurrency=(1 2 4 8 12 24 48)
declare -a recordSize=(4KB)
echo -e "Workload \t Concurrency \t RecordSize \t Time(sec)" > output_4kb.txt
for ((i=0;i<${#concurrency[*]};i++))
do
    for ((j=0;j<${#workload[*]};j++))
    do
        ./diskBench ${recordSize[0]} ${concurrency[$j]} ${workload[$i]} >> output_4kb.txt
        echo "---------------------------------------------------------" >> output_4kb.txt
    done
    rm -rf [0-9]*.txt
done
