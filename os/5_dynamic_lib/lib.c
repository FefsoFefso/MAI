#include "lib.h"

Lst* init(Lst* list){
    list = malloc(sizeof(Lst));
    list -> size = 0;
    list -> lst = malloc(sizeof(TNode));
    list -> lst -> next = list -> lst -> prev = NULL;
    return list;
}

bool isEmpty(Lst* list){ 
    return list -> lst -> next == NULL ? true : false; 
}

void printList(Lst* list) {
    if (list -> size == 0) {
        printf("List is empty\n");
    } else {
        int cur = 0;
        while (list -> lst -> next != NULL) {
            if (cur == 0) {
                continue;
            } else {
                ++cur;
            }
            printf("%s ", list -> lst -> value);
            list -> lst = list -> lst -> next;
        }
    }
}

Lst* insert(Lst* list, int pos, char* val) {
    if (pos < 1 || pos > list -> size) {
        printf("Incorrect place\n");
        return list;
    } else {
        int curPos = 0;
        while (curPos != pos) {
            list -> lst = list -> lst -> next;
            ++curPos;
        }
        TNode* newElem = malloc(sizeof(TNode));
        newElem -> value = malloc(32 * sizeof(char));
        newElem -> value = val;
        newElem -> prev = list -> lst -> prev;
        newElem -> next = list -> lst;
        list -> lst -> prev -> next = newElem;
        list -> lst -> prev = newElem;
        ++list -> size;
    }
    return list;
}

Lst* erase(Lst* list, int pos){
    if (pos < 1 || list -> size < pos) {
        printf("Incorrect place\n");
        return list;
    } else {
        int curPos = 0;
        while (curPos != pos) {
            list -> lst = list -> lst -> next;
            ++curPos;
        }
        TNode* cur = list -> lst;
        list -> lst -> prev = list -> lst -> next;
        --list -> size;
        free(cur);
    }
    return list;
}

Lst* deleteList(Lst* list){
    while (list -> lst -> next != NULL) {
        list -> lst = list -> lst -> next;
        free(list -> lst -> prev);
    }
    free(list -> lst);
    free(list);
    list = NULL;
}
