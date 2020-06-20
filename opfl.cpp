//
// Created by 张译 on 2020/6/15.
//

#include "filesys.h"

int opfl(int user_id, char *filename, int mode) {
    int pos = name_is_exist(filename);
    int user_ind = 0;
    if (pos == -1) {
        ErrorHandling("File not exist!");
    }
    inode *pre_inode = iget(curdir.direct[pos].d_ino);
    if (pre_inode->dinode.di_uid != curdir.user_id && pre_inode->dinode.di_number == 1) {
        iput(pre_inode);
        ErrorHandling("Permission denied!");
    }
    if(pre_inode->dinode.di_mode!=DIFILE)
    {
        iput(pre_inode);
        ErrorHandling("It is not a file!");
    }
    for (; user_ind < USERNUM; user_ind++)
        if (users[user_ind].u_uid == user_id)
            break;
    int syspos, userpos;
    for (syspos = 0; syspos < SYSOPENFILE; syspos++) {
        if (sysopen_file[syspos].f_count == 0)
            break;
    }
    if (syspos == SYSOPENFILE) {
        iput(pre_inode);
        ErrorHandling("System open too much file! ");
    }

    for (userpos = 0; userpos < USEROPENFILE; userpos++)
        if (users[user_ind].u_ofile[userpos] == SYSOPENFILE + 1)
            break;
    if (userpos == USEROPENFILE) {
        iput(pre_inode);
        ErrorHandling("User open too much file!");
    }
    users[user_ind].u_ofile[userpos] = syspos;
    users[user_ind].open_num++;
    sysopen_file[syspos].f_count = 1;
    sysopen_file[syspos].f_flag = mode;
    sysopen_file[syspos].f_inode = pre_inode;
    sysopen_file[syspos].f_off = 0;
    return userpos;
}