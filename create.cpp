//
// Created by 张译 on 2020/6/13.
//

#include "filesys.h"

int create(int user_id, char *name) {
    int pos = name_is_exist(name);
    int user_ind = 0;

    for (user_ind = 0; user_ind < USERNUM; user_ind++) {
        if (users[user_ind].u_uid == user_id)
            break;
    }


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

        int syspos, userpos;
        for (syspos = 0; syspos < SYSOPENFILE; syspos++) {
            if (sysopen_file.f_count == 0)
                break;
        }
        if (syspos == SYSOPENFILE) {
            iput(tmp);
            ErrorHandling("System open too much file! ")
        }

        for (userpos = 0; userpos < USEROPENFILE; userpos++)
            if (users[user_ind].u_ofile[userpos] == SYSOPENFILE + 1)
                break;
        users[user_ind].u_ofile[userpos] = syspos;
        users[user_ind].open_num++;
        sysopen_file[syspos].f_count=1;
        sysopen_file[syspos].f_flag=DIDIR;
        sysopen_file[syspos].f_inode=tmp;
        sysopen_file[syspos].f_off=0;
        return userpos;
    } else {

    }
}