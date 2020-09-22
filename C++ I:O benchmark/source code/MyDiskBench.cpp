#include <iostream>
#include <pthread.h>
#include <time.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "sequentialRead.h"
#include "sequentialWrite.h"
#include "randomRead.h"
#include "randomWrite.h"
using namespace std;
struct arguments
{
    const char *fname;
    long fileSize;
    int recordSize;
    int threadCount;
    string accessPat;
    string bufSize;
};
int main(int argc, char *argv[])
{
    int recordSize, threadCount;
    string accessPat;
    string bufSize;
    //get command line arguments
    for (int i = 0; i < argc; i++)
    {
        if (i == 1)
        {
            bufSize = argv[i];
            for (int j = 0; j < bufSize.length(); j++)
            {
                if (bufSize[j] == 'M' || bufSize[j] == 'm')
                {
                    recordSize = atoi(argv[i]) * 1024 * 1024;
                }
                else if (bufSize[j] == 'K' || bufSize[j] == 'k')
                {
                    recordSize = atoi(argv[i]) * 1024;
                }
            }
        }
        else if (i == 2)
        {
            threadCount = atoi(argv[i]);
        }
        else if (i == 3)
        {
            accessPat = argv[i];
        }
    }
    //array for different file size according to no. of threads used in bytes for 10GB of data
    long fsize[] = {10737418240, 5368709120, 2684354560, 1342177280, 873809839, 436910162, 218449839};
    //variable to access record size according to thread count
    int pos;
    if (threadCount == 1)
    {
        pos = 0;
    }
    else if (threadCount == 2)
    {
        pos = 1;
    }
    else if (threadCount == 4)
    {
        pos = 2;
    }
    else if (threadCount == 8)
    {
        pos = 3;
    }
    else if (threadCount == 12)
    {
        pos = 4;
    }
    else if (threadCount == 24)
    {
        pos = 5;
    }
    else if (threadCount == 48)
    {
        pos = 6;
    }
    pthread_t tid[threadCount];
    string ext = ".txt";
    //perform sequential/random read/write operation according to the access pattern
    if (accessPat.compare("WS") == 0 || accessPat.compare("ws") == 0) //Sequential Write operation
    {
        for (int i = 0; i < threadCount; i++)
        {
            struct arguments *obj = (struct arguments *)malloc(sizeof(struct arguments));
            char str[21];
            sprintf(str, "%d", i);
            std::string s = str + ext;
            const char *fm = s.c_str();
            obj->fname = fm;
            obj->fileSize = fsize[pos];
            obj->recordSize = recordSize;
            obj->threadCount = threadCount;
            obj->accessPat = accessPat;
            obj->bufSize = bufSize;
            pthread_create(&tid[i], NULL, seqWrite, (void *)obj);
            pthread_join(tid[i], NULL);
        }
    }
    else if (accessPat.compare("WR") == 0 || accessPat.compare("wr") == 0) //Random Write operation
    {
        for (int i = 0; i < threadCount; i++)
        {
            struct arguments *obj = (struct arguments *)malloc(sizeof(struct arguments));
            char str[21];
            sprintf(str, "%d", i);
            std::string s = str + ext;
            const char *fm = s.c_str();
            obj->fname = fm;
            obj->fileSize = fsize[pos];
            obj->recordSize = recordSize;
            obj->threadCount = threadCount;
            obj->accessPat = accessPat;
            obj->bufSize = bufSize;
            pthread_create(&tid[i], NULL, ranWrite, (void *)obj);
            pthread_join(tid[i], NULL);
        }
    }
    else if (accessPat.compare("RS") == 0 || accessPat.compare("rs") == 0) //Sequential Read operation
    {
        for (int i = 0; i < threadCount; i++)
        {
            struct arguments *obj = (struct arguments *)malloc(sizeof(struct arguments));
            char str[21];
            sprintf(str, "%d", i);
            std::string s = str + ext;
            const char *fm = s.c_str();
            obj->fname = fm;
            obj->fileSize = fsize[pos];
            obj->recordSize = recordSize;
            obj->threadCount = threadCount;
            obj->accessPat = accessPat;
            obj->bufSize = bufSize;
            pthread_create(&tid[i], NULL, seqRead, (void *)obj);
            pthread_join(tid[i], NULL);
        }
    }
    else if (accessPat.compare("RR") == 0 || accessPat.compare("rr") == 0) //Random Read operation
    {
        for (int i = 0; i < threadCount; i++)
        {
            struct arguments *obj = (struct arguments *)malloc(sizeof(struct arguments));
            char str[21];
            sprintf(str, "%d", i);
            std::string s = str + ext;
            const char *fm = s.c_str();
            obj->fname = fm;
            obj->fileSize = fsize[pos];
            obj->recordSize = recordSize;
            obj->threadCount = threadCount;
            obj->accessPat = accessPat;
            obj->bufSize = bufSize;
            pthread_create(&tid[i], NULL, ranRead, (void *)obj);
            pthread_join(tid[i], NULL);
        }
    }

    pthread_exit(NULL);
    return 0;
}
