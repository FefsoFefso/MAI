#include <windows.h>
#include <stdio.h>
#include "lib.h"

void menu() {
    printf("1)insert 2)delete 3)print 4)is_empty?\n");
}

int main(){
    Lst* list = NULL;
    list = init(list);
    int cmd;
    menu();
    while (1){
        scanf("%d", &cmd);
        if (cmd == 1) {
            printf("Enter pos:\n");
            int pos;
            scanf("%d", &pos);
            printf("Enter key (md5):\n");
            char value[32];
            scanf("%s", value);
            list = insert(list, pos, value);
        } else if (cmd == 2) {
            printf("Enter pos:\n");
            int pos;
            scanf("%d", &pos);
            list = erase(list, pos);
        } else if (cmd == 3) {
            printList(list);
        } else if (cmd == 4) {
            if (isEmpty(list)) {
                printf("Empty\n");
            } else { 
                printf("Not empty\n");
            }
        } else {
            break;
        }
        menu();
    }
    list = deleteList(list);
    return 0;
}
