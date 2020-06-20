//
// Created by 张译 on 2020/6/13.
//

#include "filesys.h"

int name_is_exist(char *name) {
    for (int i = 0; i < curdir.size / sizeof(ddd); i++) {
        if (!strcmp(curdir.direct[i].d_name, name) && curdir.direct[i].d_ino != -1) {
            return i;
        }
    }
    return -1;
}

int dir_alloc_name(char *name) {
    int res = curdir.size / sizeof(direct);
    if (res == DIRNUM) {
        ErrorHandling("Current dir is full!");
    } else {
        strcpy(curdir.direct[res].d_name, name);
        curdir.size += sizeof(direct);
        auto i = iget(curdir.direct[0].d_ino);
        i->dinode.di_size += sizeof(direct);
        iput(i);
    }
    return res;
}