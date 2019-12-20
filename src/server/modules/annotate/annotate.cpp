#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"

#include "../../../FileProcesser.h"
#include "../../../utils/LinkedList.h"

extern FileProcesser Processer;
extern pthread_spinlock_t slock_one_client; // defined in main.cpp

int ANNOTATE_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	//printf("DATA: %s\n",data);

	/* Cloning does not work ok */
	/*
	std::map <Match, int> Matches;
	Writer writer;
	AhoCorasick DictApproxMatch(&Matches), DictPerfectMatch(&Matches);
	FileProcesser *proc=Processer.clone(&Matches, &writer, &DictApproxMatch, &DictPerfectMatch);
        DictApproxMatch.InitializeAho();
        DictPerfectMatch.InitializeAho();
        DictApproxMatch.BfsFail();
        DictPerfectMatch.BfsFail();

	
	delete proc;*/
	

	LinkedList *list=LinkedList_new();
	
	pthread_spin_lock(&slock_one_client);
	try{
	    Processer.ProcessFromString((const char*)data,list);
	}catch(std::string e){
	    logMessage(0,e);
	}
	pthread_spin_unlock(&slock_one_client);

	
	*ret_data=(unsigned char*)LinkedList_toString(list);
	LinkedList_free(&list);
	*ret_sz=strlen((const char*)(*ret_data));

	return 0;
}
