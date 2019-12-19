#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include "module.h"

#include "../../../FileProcesser.h"
#include "../../../utils/LinkedList.h"

extern FileProcesser Processer;

int ANNOTATE_handler(StrMap *args, const unsigned char *data,unsigned long data_sz,Encoding *enc,unsigned char **ret_data,unsigned long *ret_sz){
	if(args==NULL || ret_data==NULL)return -1;

	//printf("DATA: %s\n",data);

	LinkedList *list=LinkedList_new();
	Processer.ProcessFromString((const char*)data,list);
	*ret_data=(unsigned char*)LinkedList_toString(list);
	LinkedList_free(&list);
	*ret_sz=strlen((const char*)(*ret_data));

	return 0;
}
