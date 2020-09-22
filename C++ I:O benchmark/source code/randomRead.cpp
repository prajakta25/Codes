#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdlib>
#include "utils.h"
#include "randomRead.h"
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
//function to perform random read
void *ranRead(void *vp)
{
    const char *fileName = ((struct args *)vp)->fname;
    long fileSize = ((struct args *)vp)->fileSize;
    int recordSize = ((struct args *)vp)->recordSize;
    int threadCount = ((struct args *)vp)->threadCount;
    string accessPat = ((struct args *)vp)->accessPat;
    string bufSize = ((struct args *)vp)->bufSize;
    //declare read buffer
    char *rbuffer = NULL;
    //allocate memory to read buffer
    posix_memalign((void **)&rbuffer, 4096, recordSize);
    int f = open(fileName, O_SYNC | O_DIRECT | O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);
    if (f == -1)
    {
        cout << "file cannot be opened" << endl;
    }
    //set position of file descriptor to a random position using fseek
    FILE *fp = fdopen(f, "r");
    fseek(fp, random(fileSize, recordSize), SEEK_SET);
    long sum = 0.0;
    //start reading from the file
    int counter = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (sum <= fileSize)
    {
        sum += read(fileno(fp), rbuffer, recordSize);
        fseek(fp, random(fileSize, recordSize), SEEK_SET);
        counter++;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //calculate time taken
    double timeTaken = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0;
    cout << accessPat << "\t" << threadCount << "\t" << bufSize << "\t" << timeTaken << endl;
    close(f);
    free(rbuffer);
    return NULL;
}