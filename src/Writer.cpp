#include "Writer.h"

void Writer::PrintCh(char c, FILE *fout){
    bufw[pbufw++] = c;
    if(pbufw == BUF_SIZE){
        if(fwrite(bufw, 1, BUF_SIZE, fout) != BUF_SIZE)
            throw std::string("Could not write in file");
        pbufw = 0;
    }
}
void Writer::PrintNum(int a, FILE *fout){
    if(a > 0){
        PrintNum(a / 10, fout);
        PrintCh(a % 10 + '0', fout);
    }
}

void logMessage(int msgType, std::string msg){
    time_t rawtime;
    struct tm *info;
    char buffer[80];

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y/%m/%d %H:%M:%S", info);

    if(msgType <= LOG_LEVEL && msg.size())
        fprintf(stderr, "%d %s %s\n", msgType, buffer, msg.c_str());
    else if(!msg.size())
        fprintf(stderr, "ERROR\n");
}
