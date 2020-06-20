//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void del(char *filename) {
    unsigned int dirid;
    inode *inode;
    int iReturn_value;
    int i;

    dirid = name_is_exist(filename);

    if (dirid != NULL)
        inode = iget(curdir.direct[dirid].d_ino);
    else
        ErrorHandling("No such file");

    for(int i=0;i<SYSOPENFILE;i++)
    {
        if(sysopen_file[i].f_count&&sysopen_file[i].f_inode==inode)
        {
            iput(inode);
            ErrorHandling("This file has been opened. Please close it to delete.");
        }
    }

    inode->dinode.di_number--;

    iReturn_value = iput(inode);
    if (iReturn_value == 1) {
        strcpy(curdir.direct[dirid].d_name, "");
        curdir.direct[dirid].d_ino = -1;

        for (; dirid <= curdir.size - 1; dirid++)
            if (curdir.direct[dirid + 1].d_ino != -1) {
                memcpy(&curdir.direct[dirid], &curdir.direct[dirid + 1], sizeof(struct direct));
               curdir.direct[dirid+1].d_ino=-1;
            } else
                break;
        curdir.size -= sizeof(direct);
    }
}