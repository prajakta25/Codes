#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <ostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <queue>
#include <limits>
#define MAX_MEM_SIZE 8589934500
#define MAX_SIZE_GB 8
using namespace std;
long counter = 0.0l;
vector<string> array;
struct arguments
{
    vector<string> arr;
    int left;
    int right;
    int threadCount;
    int partition;
    int id;
};
struct ioArgs
{
    long rbufsz;
    long wbufsz;
    int offset;
    char *filename;
    int id;
    int threadCount;
};
struct MinHeapNode
{
    string line;
    int i;
};
struct compare
{
    bool operator()(const MinHeapNode a, const MinHeapNode b) const
    {
        return (a.line.compare(b.line) > 0);
    }
};

//function to return size of file in bytes
streampos getFileSize(string f)
{
    streampos s = 0;
    ifstream ob(f, ios::in);
    s = ob.tellg();
    ob.seekg(0, ios::end);
    s = ob.tellg() - s;
    ob.close();
    return s;
}

//function to apply check on the memory usage
int memory_utilization_check(int size)
{
    if (size <= MAX_MEM_SIZE)
    {
        return 0;
    }
    return 1;
}

void merge(vector<string> &arr, int left, int mid, int right)
{
    int new_l = mid - left + 1;
    int new_r = right - mid;
    vector<string> left_arr;
    vector<string> right_arr;
    left_arr.resize(new_l);
    right_arr.resize(new_r);
    for (int i = 0; i < new_l; i++)
    {
        left_arr.at(i) = arr.at(left + i);
    }

    for (int j = 0; j < new_r; j++)
    {
        right_arr.at(j) = arr.at(mid + 1 + j);
    }
    int i = 0, j = 0, k = left;
    while (i < new_l && j < new_r)
    {
        if ((left_arr.at(i).compare(right_arr.at(j)) < 0) || (left_arr.at(i).compare(right_arr.at(j)) == 0))
        {
            arr.at(k) = left_arr.at(i);
            i++;
        }
        else
        {
            arr.at(k) = right_arr.at(j);
            j++;
        }
        k++;
    }
    while (i < new_l)
    { //extra line in left array
        arr.at(k) = left_arr.at(i);
        i++;
        k++;
    }
    while (j < new_r)
    { //extra line in right array
        arr.at(k) = right_arr.at(j);
        j++;
        k++;
    }
}
void mergeSort(vector<string> &arr, int left, int right)
{
    int mid = 0;
    if (left < right)
    {
        mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
//parallel merge sort function
void *p_inplace_merge_sort(void *args)
{

    int thread_id = ((struct arguments *)args)->id;
    int partition = ((struct arguments *)args)->partition;
    int threadCnt = ((struct arguments *)args)->threadCount;
    int offset = array.size() % threadCnt;
    int left = thread_id * (partition);
    int right = (thread_id + 1) * (partition)-1;
    if (thread_id == threadCnt - 1)
    {
        right += offset;
    }
    int middle = left + (right - left) / 2;
    if (left < right)
    {
        mergeSort(array, left, right);
        mergeSort(array, left + 1, right);
        merge(array, left, middle, right);
    }
}

//in place merge sort
void inplace_merge_sort(const char *fileName, int threadCount)
{
    pthread_t tid[threadCount];
    struct arguments *obj;
    if (memory_utilization_check(counter) == 0)
    {
        obj = (struct arguments *)malloc(sizeof(struct arguments));
        counter += sizeof(struct arguments);
    }
    else
    {
        exit(-1);
    }
    obj->left = 0;
    obj->right = array.size() - 1;
    obj->threadCount = threadCount;
    obj->partition = array.size() / threadCount;
    for (int i = 0; i < threadCount; i++)
    {
        obj->id = i;
        pthread_create(&tid[i], NULL, p_inplace_merge_sort, (void *)obj);
    }

    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(tid[i], NULL);
    }
    mergeSort(array, 0, array.size() - 1);
    free(obj);
    counter -= sizeof(struct arguments);
}

// Merges k sorted files.
void kway_merge(char *output_file, int n, int k)
{
    int in[k];
    for (int i = 0; i < k; i++)
    {
        char fileName[2];
        string ext = "_sorted.txt";
        sprintf(fileName, "%d", i);
        string s = fileName + ext;
        in[i] = open(s.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
    }
    int c = 0;
    //output file
    int out = open(output_file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);

    //creating minheap
    MinHeapNode harr[k];
    priority_queue<MinHeapNode, vector<MinHeapNode>, compare> pq;

    for (int i = 0; i < k; i++)
    {
        //read first line of each file
        char *buf;
        if (memory_utilization_check(counter) == 0)
        {
            buf = (char *)malloc(sizeof(char) * 100);
            counter += sizeof(char) * 100;
        }
        else
        {
            exit(-1);
        }
        read(in[i], buf, 100);
        //store in respective heap
        harr[i].line = buf;
        harr[i].i = i;
        pq.push(harr[i]);
        free(buf);
        counter = -sizeof(char) * 100;
    }

    int count = 0;
    while (count != k)
    {
        MinHeapNode root = pq.top();
        pq.pop();
        char *wbuf = (char *)root.line.c_str();
        write(out, wbuf, 100);
        char *rbuf;
        if (memory_utilization_check(counter) == 0)
        {
            rbuf = (char *)malloc(sizeof(char) * 100);
            counter += sizeof(char) * 100;
        }
        else
        {
            exit(-1);
        }
        //look for the next min element
        if (read(in[root.i], rbuf, 100) == 0)
        {
            root.line = "empty";
            count++;
        }
        else
        {
            root.line = rbuf;
        }
        if (root.line.compare("empty") != 0)
        {
            pq.push(root);
        }
        free(rbuf);
        counter -= sizeof(char) * 100;
    }
    close(out);
    // close input and output files
    for (int i = 0; i < k; i++)
        close(in[i]);
}

void readData(const char *fileName, int threadCount)
{
    array.clear();
    char *buf;
    if (memory_utilization_check(counter) == 0)
    {
        buf = (char *)malloc(sizeof(char) * 100);
        counter += sizeof(char) * 100;
    }
    else
    {
        exit(-1);
    }
    int f = open(fileName, O_RDONLY, S_IRUSR | S_IWUSR);
    vector<string>::iterator it;
    long sum = 0.0;
    string fn(fileName);

    while (sum < getFileSize(fn))
    {
        sum += read(f, buf, 100);
        string str(buf);
        array.push_back(str);
    }
    free(buf);
    counter -= sizeof(buf);
}
void writeData(const char *fileName)
{
    char *fm = strtok((char *)fileName, ".");
    char str[21];
    sprintf(str, "%s", fm);
    string ext = "_sorted.txt";
    std::string s = str + ext;
    ofstream wfile(s);
    for (const auto &e : array)
    {
        wfile << e;
    }
}
//function to perform random read
void *readWrite(void *vp)
{
    char *fileName = ((struct ioArgs *)vp)->filename;
    long recordSize = ((struct ioArgs *)vp)->rbufsz;
    int offset = ((struct ioArgs *)vp)->offset;
    long wbuf = ((struct ioArgs *)vp)->wbufsz;
    int id = ((struct ioArgs *)vp)->id;
    int threadCount = ((struct ioArgs *)vp)->threadCount;
    //declare read buffer
    char *rbuffer = NULL;
    //allocate memory to read buffer
    if (memory_utilization_check(counter) == 0)
    {
        rbuffer = (char *)malloc(sizeof(char) * 100);
        counter += recordSize;
    }
    else
    {
        exit(-1);
    }
    //read file descriptor
    int f = open(fileName, O_RDONLY, S_IRUSR | S_IWUSR);
    if (f == -1)
    {
        cout << "file cannot be opened" << endl;
    }
    string ext = ".txt";
    char str[21];
    sprintf(str, "%d", id);
    std::string s = str + ext;
    const char *fm = s.c_str();
    //write file descriptor
    int fd = open(fm, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1)
    {
        cout << "file cannot be opened";
    }
    //set position of file pointer using fseek
    FILE *fp = fdopen(f, "r");
    fseek(fp, offset, SEEK_SET);
    //start reading and writing from/to the file
    long sum = 0.0;
    while (sum < recordSize)
    {
        sum += read(fileno(fp), rbuffer, 100);
        write(fd, rbuffer, 100);
    }
    close(f);
    close(fd);
    free(rbuffer);
    counter -= recordSize;
    return NULL;
}
int main(int argc, char *argv[])
{
    int threadCount;
    int chunks;
    long readBufSize;
    long writeBufSize;
    char *fileName;
    counter = 0;
    //get command line arguments
    for (int i = 0; i < argc; i++)
    {
        if (i == 1)
        {
            threadCount = atoi(argv[i]);
        }
        else if (i == 2)
        {
            fileName = argv[i];
        }
    }

    //select in-place merge sort or external merge sort based on file size
    if ((getFileSize(fileName)) / 1024 / 1024 / 1024 < MAX_SIZE_GB)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        //read data from file
        readData(fileName, threadCount);
        //perform in place merge sort
        inplace_merge_sort(fileName, threadCount);
        //write data to output file
        writeData(fileName);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double timeTaken = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
        cout << "Time : " << timeTaken << " sec" << endl;
    }
    else
    {
        //perform external sort
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        //calcuate the chunks according to fileSize
        chunks = (getFileSize(fileName) / MAX_MEM_SIZE);

        //calculate the size of read buffer
        readBufSize = (double)(MAX_SIZE_GB / (chunks + 1)) * 1024 * 1024 * 1024;
        //calculate the size of write buffer
        writeBufSize = (MAX_MEM_SIZE - (readBufSize * chunks));
        //creating threads for i/o operation
        pthread_t ioThreads[chunks];
        struct ioArgs *ioObj;
        counter += sizeof(struct ioArgs);
        if (memory_utilization_check(counter) == 0)
        {
            ioObj = (struct ioArgs *)malloc(sizeof(struct ioArgs));
        }
        else
        {
            exit(-1);
        }
        int offset = 0;
        //each thread will read max memory size allowed from the file and write to the file
        for (int i = 0; i < chunks; i++)
        {
            ioObj->filename = fileName;
            ioObj->offset = offset;
            ioObj->rbufsz = MAX_MEM_SIZE;
            ioObj->wbufsz = MAX_MEM_SIZE;
            ioObj->id = i;
            ioObj->threadCount = threadCount;
            counter -= sizeof(struct ioArgs);
            pthread_create(&ioThreads[i], NULL, readWrite, (void *)ioObj);
            pthread_join(ioThreads[i], NULL);
            offset = offset + MAX_MEM_SIZE;
        }
        string ext = ".txt";
        //perform merge sort on each chunk
        for (int i = 0; i < chunks; i++)
        {
            char str[21];
            sprintf(str, "%d", i);
            std::string s = str + ext;
            const char *fm = s.c_str();
            readData(fm, threadCount);
            inplace_merge_sort(fm, threadCount);
            writeData(fm);
        }
        //perform kway merge on the sorted chunks
        kway_merge((char *)"output_sorted.txt", array.size(), chunks);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double timeTaken = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
        cout << "Time : " << timeTaken << " sec" << endl;
    }

    pthread_exit(NULL);
    return 0;
}
