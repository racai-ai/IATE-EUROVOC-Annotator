#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedList.h"

unsigned long LinkedList_get_count(LinkedList* list){
    return list->count;
}

unsigned long LinkedList_get_size(LinkedList* list){
    return list->total_sz;
}

LinkedList *LinkedList_new(){
    LinkedList *list=(LinkedList*)malloc(sizeof(LinkedList));
    memset(list,0,sizeof(LinkedList));
    return list;
}

int LinkedList_add(LinkedList *list, const void *value, unsigned long value_sz){
    LinkedList *item;
    unsigned long sz;
    
    if(list==NULL)return 0;
    
    if(value==NULL)return 0;
    if(value_sz==0)sz=strlen((const char*)value)+1;
    else sz=value_sz;
    
    if(list->count==0){ // this is the first item
	item=list;
    }else{
	item=LinkedList_new();
	if(list->next==NULL)list->next=item;
	else list->last->next=item;
	list->last=item;
    }
    
    item->value_sz=sz;
    item->value=(void*)malloc(sz);
    memcpy(item->value,value,sz);
    
    list->count++;
    list->total_sz+=sz;
    
    return 1;
}


int LinkedList_addString(LinkedList *list, const char *str){
    return LinkedList_add(list,str,0);
}

char *LinkedList_toString(LinkedList *list){
    if(list==NULL)return NULL;
    
    char *output=(char*)malloc((list->total_sz+1)*sizeof(char));
    memset(output,0,(list->total_sz+1)*sizeof(char));
    for(LinkedList *item=list;item!=NULL;item=item->next){
	if(item->value!=NULL){
	    strcat(output,(char*)item->value);
	}
    }
    
    return output;
}

void LinkedList_free(LinkedList **listPtr){
    if(listPtr==NULL)return ;
    
    LinkedList *list=*listPtr;
    if(list==NULL) return;
    
    LinkedList *itemNext=NULL;
    for(LinkedList *item=list;item!=NULL;item=itemNext){
	if(item->value!=NULL){
	    free(item->value);
	}
	itemNext=item->next;
	free(item);
    }
    
    *listPtr=NULL;
}

    