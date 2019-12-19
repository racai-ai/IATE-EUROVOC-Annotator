#include "FileProcesser.h"

int FileProcesser::ReadFile(std::string pathInput){//citeste fisierul din pathInput
    FILE*fin = fopen(pathInput.c_str(), "r");
    if(fin == NULL) throw std::string("Could not open ") + pathInput;
    int next = 0;
    while(!feof(fin)){
        fgets(fileChars + next, INF, fin);
        next += strlen(fileChars + next);
        if(fileChars[next - 2] == '\n' && fileChars[next - 1] == '\r') next--;
        else if(fileChars[next - 1] == '\r') fileChars[next - 1] = '\n';
        else if(fileChars[next - 2] == '\r' && fileChars[next - 1] == '\n') fileChars[next - 2] = '\n', next--;
    }
    fclose(fin);
    return next;
}
void FileProcesser::ProcessCh(char &prevc, char c, int &caps, int currLine){//introduce un caracter in Aho
    DictApproxMatch -> ApproxMatchFeedCh(prevc, c, currLine);
    DictPerfectMatch -> PerfectMatchFeedCh(prevc, c, caps, currLine);
    prevc = c;
}
void FileProcesser::ProcessNonLemmatised(){//proceseaza fisierul nelematizat
    DictApproxMatch -> Reset();
    DictPerfectMatch -> Reset();

    char prevc = 0;
    int currLine = 1, caps = 1;
    for(int i = 0; i < charCount; i++){
        if('0' <= fileChars[i] && fileChars[i] <= '9'){//daca linia corespunde unui termen
            while(fileChars[i] != '\t') i++;
            i++;
            while(fileChars[i] != '\t') ProcessCh(prevc, fileChars[i++], caps, currLine);
            ProcessCh(prevc, ' ', caps, currLine);
        }
        while(fileChars[i] != '\n' && i < charCount) i++;
        currLine++;
    }
}
void FileProcesser::ProcessLemmatised(){//proceseaza fisierul lematizat
    DictApproxMatch -> Reset();
    DictPerfectMatch -> Reset();

    char prevc = 0;
    int currLine = 1, caps = 1;
    for(int i = 0; i < charCount; i++){
        if('0' <= fileChars[i] && fileChars[i] <= '9'){//daca linia corespunde unui termen
            while(fileChars[i] != '\t') i++;
            i++;
            while(fileChars[i] != '\t') i++;
            i++;
            while(fileChars[i] != '\t') ProcessCh(prevc, fileChars[i++], caps, currLine);
            ProcessCh(prevc, ' ', caps, currLine);
        }
        while(fileChars[i] != '\n' && i < charCount) i++;
        currLine++;
    }
}



int FileProcesser::ProcessMatches(std::string pathOutput){
    return ProcessMatches(1,pathOutput,NULL);
}


int FileProcesser::ProcessMatches(int fileOutput, std::string pathOutput, LinkedList *outputList){
    FILE*fout = NULL;
    if(fileOutput)
        fout=fopen(pathOutput.c_str(),"w");

    MatchesVec.clear();
    ActiveMatchesVec.clear();
    int id = 0;

    std::map <int, int> Cat;
    Cat.clear();
    for(auto y: (*Matches)) MatchesVec.push_back({y.first.b, y.first.e, y.first.index, ++id});

    int currLine = 1, nextMatch = 0;;
    for(int i = 0; i < charCount; i++){
        if(fileChars[i] < '0' || fileChars[i] > '9')//daca randul curent nu corespunde unui termen
            while(fileChars[i] != '\n' && i < charCount) writer -> PrintCh(fileChars[i++], fout, outputList);
        else{//daca randul curent corespunde unui termen
            while(fileChars[i] != '\n' && i < charCount) writer -> PrintCh(fileChars[i++], fout, outputList);
            writer -> PrintCh('\t', fout, outputList);

            //printarea tag-urilor IATE
            int mtc = 0;
            for(auto y: ActiveMatchesVec){
                if(mtc) writer -> PrintCh(';', fout, outputList);
                writer -> PrintNum(y.id, fout, outputList), writer -> PrintCh(':', fout, outputList), writer -> PrintNum((IATEid[y.index]), fout, outputList);
                mtc = 1;
            }
            if(!mtc) writer -> PrintCh('_', fout, outputList);
            writer -> PrintCh('\t', fout, outputList);
            //printarea tag-urilor EuroVoc
            mtc = 0;
            for(auto y: ActiveMatchesVec){
                if(mtc) writer -> PrintCh(';', fout, outputList);
                writer -> PrintNum(y.id, fout, outputList), writer -> PrintCh(':', fout, outputList);
                int comma = 0;
                for(auto z: EuroVocid[IATEid[y.index]]){
                    //contorizarea termenulul pentru statistica
                    if(z >= 100) Cat[z / 100]++;
                    else Cat[z]++;

                    if(comma) writer -> PrintCh(',', fout, outputList);
                    if(z >= 1000) writer -> PrintNum(z, fout, outputList);
                    else if(z >= 100) writer -> PrintCh('0', fout, outputList), writer -> PrintNum(z, fout, outputList);
                    else if(z >= 10) writer -> PrintNum(z, fout, outputList);
                    else writer -> PrintCh('0', fout, outputList), writer -> PrintNum(z, fout, outputList);
                    comma = 1;
                }
                mtc = 1;
            }
            if(!mtc) writer -> PrintCh('_', fout, outputList);
        }

        //eliminarea termenilor care se termina
        int pos = 0;
        for(int x = 0; x < ActiveMatchesVec.size(); x++)
            if(ActiveMatchesVec[x].e > currLine)
                ActiveMatchesVec[pos++] = ActiveMatchesVec[x];
        ActiveMatchesVec.resize(pos);
        //introducerea termenilor care incep
        currLine++;
        while(nextMatch < MatchesVec.size() && MatchesVec[nextMatch].b == currLine){
            ActiveMatchesVec.push_back(MatchesVec[nextMatch]);
            nextMatch++;
        }
        writer -> PrintCh('\n', fout, outputList);
    }

    writer->Flush(fout,outputList);

    //determinarea categoriei dominante
    int maxx = 0, catMaxx = 0;
    for(auto y: Cat){
        int pond = y.second;
        if(y.first == 72) pond /= 5;//geografie
        if(pond > maxx)
            maxx = pond, catMaxx = y.first;
    }

    if(fout!=NULL)fclose(fout);
    return catMaxx;
}

FileProcesser::FileProcesser(std::map <Match, int>* M, Writer* W, AhoCorasick* DictApproxM, AhoCorasick* DictPerfectM){
    fileChars = new char[MAXFILECH];
    IATEid = new int[IATELEN];
    memset(fileChars, 0x00, MAXFILECH * sizeof(char));
    memset(IATEid, 0x00, IATELEN * sizeof(int));

    Matches = M;
    writer = W;
    DictApproxMatch = DictApproxM;
    DictPerfectMatch = DictPerfectM;
}
void FileProcesser::ReadDictNew(){
    logMessage(1, "Start processing IATE terms");
    ReadFile("resources/WordsDatabase2.out");
    int pos = 0;

    char wordApproxMatch[1 + MAXLEN];
    char wordPerfectMatch[1 + MAXLEN];
    int index = 0;

    char c = fileChars[pos];
    int wordcount = 0;
    while(c != '^'){//pentru fiecare tag din IATE
        pos += 5;

        index = 0;
        while(isdigit(fileChars[pos]))
            index = index * 10 + fileChars[pos] - '0', pos++;
        if(!index) throw std::string("Invalid IATE index");

        c = fileChars[++pos];
        while(c != '\n') c = fileChars[++pos];
        c = fileChars[++pos];

        while(c != '^'){//pentru fiecare termen din tag
            char prevc = 0, tmp;
            int cntApproxMatch = 0, cnt2ApproxMatch = 0;
            int cntPerfectMatch = 0, cnt2PerfectMatch = 0, caps = 1;
            while(c != '\n' && c != EOF){
                if(c == '-') c = ' ';
                if('a' <= c && c <= 'z') caps = 0;
                if('A' <= c && c <= 'Z') c = c - 'A' + 'a';

                tmp = DictApproxMatch -> ApproxMatchProcessCh(prevc, c, cnt2ApproxMatch);
                if(tmp > 0) wordApproxMatch[cntApproxMatch++] = tmp;
                tmp = DictPerfectMatch -> PerfectMatchProcessCh(prevc, c, cnt2PerfectMatch);
                if(tmp > 0) wordPerfectMatch[cntPerfectMatch++] = tmp;

                prevc = c;
                c = fileChars[++pos];
            }
            wordcount++;
            if(cnt2PerfectMatch - 1 < MINCONS && cntPerfectMatch - 1 >= MINCHAR) DictPerfectMatch -> InsertAhoFromString(wordPerfectMatch, cntPerfectMatch, wordcount, caps);
            if(cnt2ApproxMatch - 1 >= MINCONS) DictApproxMatch -> InsertAhoFromString(wordApproxMatch, cntApproxMatch, wordcount);

            c = fileChars[++pos];
        }

        ++pos;
        c = fileChars[++pos];
    }
    logMessage(1, "Finsh processing IATE terms");
}
void FileProcesser::ReadDictTmp(){//proceseaza informatiile EuroVoc
    logMessage(1, "Start processing EuroVoc tags");
    ReadFile("resources/WordsDatabase2.out");
    int pos = 0;

    char c = fileChars[pos];
    int index = 0, wordcount = 0;
    while(c != '^'){//pentru fiecare tag IATE
        pos += 5;

        index = 0;
        while(isdigit(fileChars[pos]))
            index = index * 10 + fileChars[pos] - '0', pos++;
        if(!index) throw std::string("Invalid IATE index");

        c = fileChars[++pos];
        int euroVocid = 0;
        while(c != '\n'){//pentru fiecare categorie EuroVoc
            if('0' <= c && c <= '9') euroVocid = euroVocid * 10 + c - '0';
            char nextc = fileChars[++pos];
            if(('0' <= c && c <= '9') && (nextc < '0'  || nextc > '9')) EuroVocid[index].push_back(euroVocid), euroVocid = 0;
            c = nextc;
        }
        c = fileChars[++pos];

        while(c != '^'){//pentru fiecare termen din tag
            while(c != '\n' && c != '\0') c = fileChars[++pos];
            wordcount++;
            IATEid[wordcount] = index;
            c = fileChars[++pos];
        }

        ++pos;
        c = fileChars[++pos];
    }
    logMessage(1, "Finsh processing EuroVoc tags");
}
int FileProcesser::ProcessFile(std::string pathInput, std::string pathOutput){//proceseaza un fisier din pathInput, afiseaza rezultatul in pathOutput, returneaza categoria dominanta
    logMessage(1, "Start Processing " + pathInput);
    (*Matches).clear();
    charCount = ReadFile(pathInput);//citirea fisierului
    ProcessNonLemmatised();//procesarea fisierului nelematizat
    ProcessLemmatised();//procesarea fisierului lematizat
    int catMaxx = ProcessMatches(pathOutput);//procesarea potrivirilor si afisarea outputului
    logMessage(1, "Finsh Processing " + pathInput);

    return catMaxx;
}

int FileProcesser::ProcessFromString(const char *input, LinkedList *outputList){//proceseaza un text din "input" cu rezultatul in outputList, returneaza categoria dominanta
    logMessage(1, "Start Processing from String");
    (*Matches).clear();
    
    charCount = strlen(input);
    strcpy(fileChars,input);
    
    ProcessNonLemmatised();//procesarea fisierului nelematizat
    ProcessLemmatised();//procesarea fisierului lematizat
    int catMaxx = ProcessMatches(0,std::string(""),outputList);//procesarea potrivirilor si afisarea outputului
    logMessage(1, "Finsh Processing from String");

    return catMaxx;
}
