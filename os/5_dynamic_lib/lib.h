#ifndef _LIST_H_ 
#define _LIST_H_ 

#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct Tnode{
    struct Tnode* next;
    struct Tnode* prev;
    char* value;
}TNode;

typedef struct List{
    int size;
    TNode *lst;
}Lst;

Lst* insert(Lst* list, int pos, char* value);
Lst* erase(Lst* list, int pos);
Lst* init(Lst* list);
Lst* deleteList(Lst* list);
bool isEmpty(Lst* list);
void printList(Lst* list);


#endif  