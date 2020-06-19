//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void install() {
    fs.seekp(0, ios::beg);
    fs.read((char *) &SuperBlock, sizeof(SuperBlock));
    memset(hinode, 0, sizeof(hinode));
    for (int i = 0; i < SYSOPENFILE; i++) {
        sysopen_file[i].f_count = 0;
        sysopen_file[i].f_inode = nullptr;
    }
    for (int i = 0; i < USERNUM; i++) {
        users[i].u_uid = -1;
        for (int j = 0; j < USEROPENFILE; j++)
            users[i].u_ofile[j] = SYSOPENFILE + 1;
    }
    inode *cur_path_inode = iget(0);

    curdir.size = cur_path_inode->dinode.di_size;

    fileread(cur_path_inode->dinode, (char *) curdir.direct, curdir.size, 0);
    for (int i = curdir.size / sizeof(direct); i < DIRNUM; i++)
        curdir.direct[i].d_ino = -1;
    curdir.user_id = -1;
}