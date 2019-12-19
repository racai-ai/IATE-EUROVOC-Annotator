#include <stdio.h>
#include <time.h>
#include <string>

#include "utils/LinkedList.h"

#define LOG_LEVEL 2
#define BUF_SIZE 1 << 17

class Writer{
public:

    char bufw[1 + BUF_SIZE];
    int pbufw = 0;

    void PrintCh(char c, FILE *fout, LinkedList *outputList);
    void PrintNum(int a, FILE *fout, LinkedList *outputList);
    void Flush(FILE *fout, LinkedList *outputList);
};

void logMessage(int msgType, std::string msg);
