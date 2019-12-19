#include "Writer.h"

void Writer::PrintCh(char c, FILE *fout, LinkedList *outputList){
    bufw[pbufw++] = c;
    if(pbufw == BUF_SIZE){
        if(fout!=NULL && fwrite(bufw, 1, BUF_SIZE, fout) != BUF_SIZE)
            throw std::string("Could not write in file");
        if(outputList!=NULL){
            bufw[BUF_SIZE]=0;
            LinkedList_addString(outputList,bufw);
        }
        pbufw = 0;
    }
}
void Writer::PrintNum(int a, FILE *fout, LinkedList *outputList){
    if(a > 0){
        PrintNum(a / 10, fout, outputList);
        PrintCh(a % 10 + '0', fout, outputList);
    }
}

void Writer::Flush(FILE *fout, LinkedList *outputList){
    if(pbufw>0){
        if(fout!=NULL && fwrite(bufw, 1, pbufw, fout) != pbufw)
            throw std::string("Could not write in file");
        if(outputList!=NULL){
            bufw[pbufw]=0;
            LinkedList_addString(outputList,bufw);
        }
        pbufw = 0;
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
