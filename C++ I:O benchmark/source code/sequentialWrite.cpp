#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include "utils.h"
#include "sequentialWrite.h"
#include <cstdlib>
#include <chrono>
using namespace std;
struct args
{
    const char *fname;
    long fileSize;
    int recordSize;
    int threadCount;
    string accessPat;
    string bufSize;
};
//function to perform sequential write
void *seqWrite(void *vp)
{
    const char *fileName = ((struct args *)vp)->fname;
    long fileSize = ((struct args *)vp)->fileSize;
    int recordSize = ((struct args *)vp)->recordSize;
    int threadCount = ((struct args *)vp)->threadCount;
    string accessPat = ((struct args *)vp)->accessPat;
    string bufSize = ((struct args *)vp)->bufSize;
    //declare write buffer
    char *wbuffer = NULL;
    //allocate memory to wite buffer
    posix_memalign((void **)&wbuffer, 4096, recordSize);
    int fd = open(fileName, O_SYNC | O_DIRECT | O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1)
    {
        cout << "file cannot be opened";
    }
    //fill the buffer with  data
    for (int i = 0; i < recordSize; i++)
    {
        wbuffer[i] = 'a';
    }
    int len = (fileSize / recordSize);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //write data into file
    for (int i = 0; i < len; i++)
    {
        write(fd, wbuffer, recordSize);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //calculate time
    double timeTaken = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
    cout << accessPat << "\t" << threadCount << "\t" << bufSize << "\t" << timeTaken << endl;
    close(fd);
    free(wbuffer);

    return NULL;
}