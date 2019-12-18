#include <stdio.h>
#include <time.h>
#include <string>

#define LOG_LEVEL 2
#define BUF_SIZE 1 << 17

class Writer{
public:

    char bufw[1 + BUF_SIZE];
    int pbufw = 0;

    void PrintCh(char c, FILE *fout);
    void PrintNum(int a, FILE *fout);
};

void logMessage(int msgType, std::string msg);
