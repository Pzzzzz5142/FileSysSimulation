//
// Created by 张译 on 2020/6/13.
//

#include "filesys.h"

int name_is_exist(char *name) {
    for (int i = 0; i < DIRSIZ; i++) {
        if (!strcmp(curdir.direct[i].d_name, name) && curdir.direct[i].d_ino != -1) {
            return i;
        }
    }
    return -1;
}

int dir_alloc_name(char *name) {
    int res = -1;
    for (int i = 0; i < DIRNUM; i++) {
        if (curdir.direct[i].d_ino == -1) {
            res = i;
            break;
        }
    }
    if (res == -1) {
        printf("Current dir is full!");
    } else {
        strcpy(curdir.direct[res].d_name, name);
    }
    return res;
}