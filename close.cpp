//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void close(unsigned int user_id, unsigned short cfd)        /*close*/
{
    struct inode *inode;
    int j, k;
    for (j = 0; j < USERNUM; j++)
        if (users[j].u_uid == user_id) {
            k = j;
            break;
        }

    inode = sysopen_file[users[k].u_ofile[cfd]].f_inode;

    iput(inode);
    sysopen_file[users[k].u_ofile[cfd]].f_count=0;
    sysopen_file[users[k].u_ofile[cfd]].f_inode = nullptr;
    users[k].u_ofile[cfd] = SYSOPENFILE + 1;
}
