#include <stdio.h>
#include <cstring>
#include <map>
#include <queue>

#include "Writer.h"

#ifndef MAXFILECH
#define MAXFILECH 100000000//len max a unui fisier procesat
#endif //MAXFILECH

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
#endif // MATCH

#define SIGMA 128 //range-ul ASCII
#define NSIGMA 45 //nr de caractere valide

class AhoCorasick{
private:
    int n, k;
    int next;
    int *prov;

    int transf[SIGMA], transf2[SIGMA];
    int countApprox, countPerfect;

    std::map <Match, int>* Matches;

    struct Trie{
        int word, len, caps;
        Trie *son[NSIGMA];
        Trie *fail, *wordfail;

        Trie(){
            word = len = caps = 0;
            memset(son, 0, sizeof(son));
        }
    };
    Trie *T = new Trie();
    Trie *S;

public:
    AhoCorasick(std::map <Match, int>* M);
    void InitializeAho();
    void BfsFail();
    void Reset();
    void InsertAhoFromString(char *word, int cnt, int index, int caps = 0);

    char ApproxMatchProcessCh(char prevc, char c, int &cnt2ApproxMatch);
    char PerfectMatchProcessCh(char prevc, char c, int &cnt2PerfectMatch);
    void AddMatches(int &cntMatches);
    void ApproxMatchFeedCh(char prevc, char c, int currLine);
    void PerfectMatchFeedCh(char prevc, char c, int &caps, int currLine);
};
