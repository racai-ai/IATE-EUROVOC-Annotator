#include <stdio.h>
#include <string>
#include <map>

#include "server/http_server.h"
#include "server/modules/all.h"

#include "FileProcesser.h"

#define MAXPATHLEN 100

#ifndef MATCH
#define MATCH
struct Match{
    int b, e, index;//linia de inceput, linia de final, id unic
    bool operator < (const Match &aux) const{//comparator
        if(b != aux.b) return b < aux.b;
        if(e != aux.e) return e < aux.e;
        return index < aux.index;
    }
};
#endif

const std::string src = "./"; //directorul cu fisiere de procesat
const std::string dest = "./"; //directorul in care sunt create fisierele de output
const std::string sorted = "./sorted.txt"; //directorul in care sunt afisate categoriile fisierelor
const std::string extDest = ".iate"; //extensia fisierelor finale

std::map <Match, int> Matches;
Writer writer;
AhoCorasick DictApproxMatch(&Matches), DictPerfectMatch(&Matches);
FileProcesser Processer(&Matches, &writer, &DictApproxMatch, &DictPerfectMatch);

void Initialise(){
    logMessage(1, "Start Structure Initialization");
    DictApproxMatch.InitializeAho();
    DictPerfectMatch.InitializeAho();

    Processer.ReadDictNew();
    Processer.ReadDictTmp();

    DictApproxMatch.BfsFail();
    DictPerfectMatch.BfsFail();

    logMessage(1, "Finsh Structure Initialization");
}

pthread_spinlock_t slock_one_client;

int main(int argc, char *argv[]){
    try{
        Initialise();
        
        if(argc==3 && strcasecmp(argv[1],"server")==0){
            int port=atoi(argv[2]);
            printf("Starting server on port %d\n",port);
            http_server_init();
    	    pthread_spin_init(&slock_one_client, 0);
            SENGINE_Server_Run(port);
            return 0;
        
        }else if(argc>1){
            printf("Syntax:\n");
            printf("    annotate\n");
            printf("       starts annotation using list.txt\n\n");
            printf("    annotate server <PORT>\n");
            printf("       starts listining on <PORT> for sentences to annotate\n");
            return -1;
        }
        
        FILE*files = fopen("list.txt","r");
        if(files == NULL) throw std::string("Could not open list.txt");

        FILE*categ = fopen(sorted.c_str(),"w");
        if(categ == NULL) throw std::string("Coult not create sorted.txt");

        char pathVec[MAXPATHLEN];
        while(!feof(files)){
            fgets(pathVec, MAXPATHLEN, files);

            std::string strA = "";
            std::string strB = "";
            int i = 0;
            while(pathVec[i] != '.' &&
                  pathVec[i] != '\n' && pathVec[i] != '\r' && pathVec[i] != '\0' && pathVec[i] != EOF)
                strA.push_back(pathVec[i]), strB.push_back(pathVec[i++]);
            while(pathVec[i] != '\n' && pathVec[i] != '\r' && pathVec[i] != '\0' && pathVec[i] != EOF)
                strA.push_back(pathVec[i++]);

            if(strA.size() == 0){ logMessage(2, "Blank file in list.txt"); break;}
            int category = Processer.ProcessFile(src + strA, dest + strB + extDest);
            fprintf(categ,"%s %02d\n", strA.c_str(), category);
        }

        fclose(files);
        fclose(categ);
    }
    catch(std::string e){
        logMessage(0, e);
        return -1;
    }

    return 0;
}
