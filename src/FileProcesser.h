#include <stdio.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>

#include "AhoCorasick.h"

#include "utils/LinkedList.h"

#define INF 1000000000//infinit
#define IATELEN 55000//dimensiunea dictionarului IATE
#define MAXLEN 1000//len max a unui termen IATE
#define MINCHAR 3 //nr min de caractere al termenilor luati in considerare
#define MINCONS 5 //limita de consoane dupa care se face separarea intre Perf/Approx Matching

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

class FileProcesser{
private:
    char *fileChars;
    int charCount;

    int *IATEid;
    std::map <int, std::vector<int>> EuroVocid;
    struct MatchVecElem{int b, e, index, id;};
    std::vector <MatchVecElem> MatchesVec;
    std::vector <MatchVecElem> ActiveMatchesVec;

    std::map <Match, int>* Matches;
    Writer *writer;
    AhoCorasick *DictApproxMatch, *DictPerfectMatch;

    int ReadFile(std::string pathInput);
    void ProcessCh(char &prevc, char c, int &caps, int currLine);
    void ProcessNonLemmatised();
    void ProcessLemmatised();
    int ProcessMatches(std::string pathOutput);
    int ProcessMatches(int fileOutput, std::string pathOutput, LinkedList *outputList);

public:
    FileProcesser(std::map <Match, int>* M, Writer* W, AhoCorasick* DictApproxM, AhoCorasick* DictPerfectM);
    void ReadDictNew();
    void ReadDictTmp();
    int ProcessFile(std::string pathInput, std::string pathOutput);
    int ProcessFromString(const char *input, LinkedList *outputList);
};
