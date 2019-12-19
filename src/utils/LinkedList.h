#ifndef _PVF_LINKED_LIST_H_
#define _PVF_LINKED_LIST_H_

typedef struct _LinkedList_struct {
    _LinkedList_struct *next;
    _LinkedList_struct *last;
    void *value;
    unsigned long value_sz;
    unsigned long total_sz;
    unsigned long count;
}LinkedList;

LinkedList *LinkedList_new();
int LinkedList_add(LinkedList *list, const void *value, unsigned long value_sz);
int LinkedList_addString(LinkedList *list, const char *value);
unsigned long LinkedList_get_count(LinkedList* list);
unsigned long LinkedList_get_size(LinkedList* list);
char *LinkedList_toString(LinkedList* list);
void LinkedList_free(LinkedList **listPtr);

#endif
