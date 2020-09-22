#!/bin/bash
declare -a workload=(WS RS WR RR)
declare -a concurrency=(1 2 4 8 12 24 48)
declare -a recordSize=(64KB 1MB 16MB)
echo -e "Workload \t Concurrency \t RecordSize \t Time(sec)" > output.txt
for ((i=0;i<${#concurrency[*]};i++))
do
    for ((j=0;j<${#recordSize[*]};j++))
    do
        for ((k=0;k<${#workload[*]};k++))
        do
            ./diskBench ${recordSize[$j]} ${concurrency[$i]} ${workload[$k]} >> output.txt
             echo "---------------------------------------------------------" >> output.txt
            if [[ "$k" -eq 3 ]]; then
                rm -rf [0-9]*.txt
            fi
        done
        rm -rf [0-9]*.txt
    done
done