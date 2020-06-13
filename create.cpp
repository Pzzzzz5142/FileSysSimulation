//
// Created by 张译 on 2020/6/13.
//

#include "filesys.h"

void create(int user_id, char *name) {
    int pos = name_is_exist(name);


    if (pos == -1) {
        int dir_num = dir_alloc_name(name);
        int blkloc = balloc();
        curdir.direct[dir_num].d_ino = blkloc;
        inode *tmp = ialloc();
        tmp->dinode.di_addr[0] = blkloc;
        tmp->dinode.di_size = 1;
        tmp->dinode.di_mode = DIDIR;
        tmp->dinode.di_uid = user_id;
        tmp->dinode.di_number = 1;
    } else {

    }
}