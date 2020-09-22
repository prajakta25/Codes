#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
using namespace std;
//function to randomly generate number for random read/write operation
long random(long fileSize, int recordSize)
{
    double n = (double)(fileSize - recordSize);
    return (long)(((double)rand() / (double)RAND_MAX) * n);
}
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