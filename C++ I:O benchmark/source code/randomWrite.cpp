#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
#include "utils.h"
#include "randomWrite.h"
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
//function to perform random write
void *ranWrite(void *vp)
{
    const char *fileName = ((struct args *)vp)->fname;
    long fileSize = ((struct args *)vp)->fileSize;
    int recordSize = ((struct args *)vp)->recordSize;
    int threadCount = ((struct args *)vp)->threadCount;
    string accessPat = ((struct args *)vp)->accessPat;
    string bufSize = ((struct args *)vp)->bufSize;
    //declare write buffer
    char *wbuffer = NULL;
    //allocate memory to write buffer
    posix_memalign((void **)&wbuffer, 4096, recordSize);
    int fd = open(fileName, O_SYNC | O_DIRECT | O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1)
    {
        cout << "file cannot be opened";
    }
    for (int i = 0; i < recordSize; i++)
    {
        wbuffer[i] = 'a';
    }
    //write into file
    int counter = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    FILE *fp = fdopen(fd, "w+");
    //write data into file until it reaches the desired size
    while ((getFileSize(fileName) / 1024 / 1024) < ((fileSize / 1024 / 1024) - 24))
    {
        //change file pointer position for random write
        fseek(fp, random(fileSize, recordSize), SEEK_SET);
        write(fileno(fp), wbuffer, recordSize);
        counter++;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //calculate time
    double timeTaken = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
    cout << accessPat << "\t" << threadCount << "\t" << bufSize << "\t" << timeTaken << endl;
    close(fd);
    free(wbuffer);
    return NULL;
}