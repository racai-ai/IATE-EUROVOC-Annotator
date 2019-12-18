#include "AhoCorasick.h"

AhoCorasick::AhoCorasick(std::map <Match, int>* M){
    prov = new int[MAXFILECH];
    memset(prov, 0x00, MAXFILECH * sizeof(int));

    countApprox = countPerfect = 0;
    Matches = M;

    transf[' '] = transf2[' '] = 1;
    transf['-'] = transf2['-'] = 1;
    for(int i = 'a'; i <= 'z'; i++)
        transf[i] = transf2[i] = i - 'a' + 2;
    for(int i = '0'; i <= '9'; i++)
        transf[i] = transf2[i] = 27 + i - '0' + 1;
    transf['a'] = transf['e'] = transf['i'] = transf['o'] = transf['u'] = 0;
}
void AhoCorasick::InitializeAho(){
    logMessage(1, "Start Aho-Corasick initialization");
    T -> fail = T -> wordfail = T;
    logMessage(1, "Finsh Aho-Corasick initialization");
}
void AhoCorasick::BfsFail(){
    logMessage(1, "Start creating Aho-Corasick fail edges");
    std::queue <Trie*> q;
    for(int i = 0; i < NSIGMA; i++)
        if(T -> son[i]){
            T -> son[i] -> fail = T;
            T -> son[i] -> wordfail = T;
            q.push(T -> son[i]);
        }

    while(!q.empty()){
        Trie *node = q.front(); q.pop();
        for(int i = 0; i < NSIGMA; i++)
            if(node -> son[i]){
                Trie *fail = node;
                do{
                    fail = fail -> fail;
                }while(fail != T && !fail -> son[i]);
                if(fail -> son[i])
                    fail = fail -> son[i];
                node -> son[i] -> fail = fail;
                q.push(node -> son[i]);
            }
        Trie *fail = node -> fail;
        if(!fail -> word)
            fail = fail -> wordfail;
        node -> wordfail = fail;
    }
    logMessage(1, "Finsh creating Aho-Corasick fail edges");
}
void AhoCorasick::Reset(){
    S = T;
    next = 0;
}
void AhoCorasick::InsertAhoFromString(char *word, int cnt, int index, int caps){
    S = T;
    for(int len = 1; len <= cnt; len++){
        char c = word[len - 1];
        if(!S -> son[c])
            S -> son[c] = new Trie();
        S = S -> son[c];
    }
    S -> word = index;
    S -> caps = caps;
    S -> len = cnt;
}

char AhoCorasick::ApproxMatchProcessCh(char prevc, char c, int &cnt2ApproxMatch){
    if(prevc == ' ' && (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')) c = transf2[c];
    else{
        if(c >= 0 && transf[c]) cnt2ApproxMatch++;
        if(c >= 0) c = transf[c];
        else if((prevc == -56 && c == -101) ||
                (prevc == -59 && c == -93)  ||
                (prevc == -59 && c == -94)) c = 38;//t
        else if((prevc == -56 && c == -103) ||
                (prevc == -59 && c == -97)  ||
                (prevc == -59 && c == -98)) c = 39;//s
        else c = 0;
    }
    return c;
}
char AhoCorasick::PerfectMatchProcessCh(char prevc, char c, int &cnt2PerfectMatch){
    if(transf[c]) cnt2PerfectMatch++;

    if(c >= 0) c = transf2[c];
    else if((prevc == -56 && c == -101) ||
            (prevc == -59 && c == -93)  ||
            (prevc == -59 && c == -94)) c = 38; //t
    else if((prevc == -56 && c == -103) ||
            (prevc == -59 && c == -97)  ||
            (prevc == -59 && c == -98)) c = 39; //s
    else if(prevc == -60 && (c == -125 || c == -126)) prevc = c, c = 40; //a (doar)
    else if(prevc == -61 && (c == -82  || c == -114)) prevc = c, c = 41; //a
    else if(prevc == -61 && (c == -94  || c == -126)) prevc = c, c = 42; //i
    else c = 0;
    return c;
}
void AhoCorasick::AddMatches(int &cntMatches){
    Trie *R = S;
    while(R != T){
        (*Matches)[{prov[next - R -> len + 1], prov[next - 1], R -> word}]++;
        cntMatches++;
        R = R -> wordfail;
    }
}
void AhoCorasick::ApproxMatchFeedCh(char prevc, char c, int currLine){
    if(c == '\n' || c == '-') c = ' ';
    if('A' <= c && c <= 'Z') c = c - 'A' + 'a';
    if(c == ' ') if(S -> word) AddMatches(countApprox);

    if(prevc == ' ' && (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')) c = transf2[c];
    else{
        if(c >= 0) c = transf[c];
        else if((prevc == -56 && c == -101) ||
                (prevc == -59 && c == -93)  ||
                (prevc == -59 && c == -94)) c = 38; //t
        else if((prevc == -56 && c == -103) ||
                (prevc == -59 && c == -97)  ||
                (prevc == -59 && c == -98)) c = 39; //s
        else c = 0;
    }

    if(c > 0){
        prov[next++] = currLine;
        while(S != T && !S -> son[c])
            S = S -> fail;
        if(S -> son[c])
            S = S -> son[c];
    }
}
void AhoCorasick::PerfectMatchFeedCh(char prevc, char c, int &caps, int currLine){
    if(c == '\n' || c == '-') c = ' ';
    if(c == ' ') if(S -> word && (S -> caps == 0 || caps == 1)) AddMatches(countPerfect);

    if(c == ' ') caps = 1;
    if('a' <= c && c <= 'z') caps = 0;

    if('A' <= c && c <= 'Z') c = c - 'A' + 'a';

    if(c >= 0) c = transf2[c];
    else if((prevc == -56 && c == -101) ||
            (prevc == -59 && c == -93)  ||
            (prevc == -59 && c == -94)) c = 38; //t
    else if((prevc == -56 && c == -103) ||
            (prevc == -59 && c == -97)  ||
            (prevc == -59 && c == -98)) c = 39; //s
    else if(prevc == -60 && (c == -125 || c == -126)) prevc = c, c = 40; //a (doar)
    else if(prevc == -61 && (c == -82  || c == -114)) prevc = c, c = 41; //a
    else if(prevc == -61 && (c == -94  || c == -126)) prevc = c, c = 42; //i
    else c = 0;

    if(c){
        prov[next++] = currLine;
        while(S != T && !S -> son[c])
            S = S -> fail;
        if(S -> son[c])
            S = S -> son[c];
    }
}
