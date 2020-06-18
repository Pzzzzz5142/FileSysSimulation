//
// Created by 张译 on 2020/6/12.
//
#include "filesys.h"

void format() {
    char buff[(FILEBLK + DINODEBLK + 1) * BLOCKSIZ];
    memset(buff, 0, sizeof(buff));
    fs.seekp(0, ios::beg);
    fs.write(buff, sizeof(buff));
    direct dir_buff[DIRNUM];

    SuperBlock.charge.ind = 0;
    SuperBlock.charge.stk[0] = -1;
    for (int i = 2; i < FILEBLK; i++)//初始化成组链接法，0为根目录，1为etc目录
    {
        bfree(i);
    }

    SuperBlock.ihead = -1;
    for (int i = 0; i < MAXINODE * DINODEBLK; i++) {
        ifree(i);
    }

    inode *tmp = iget(0);//最根的目录
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 0;
    tmp->dinode.di_addr[0] = 0;
    tmp->dinode.di_size = 3 * sizeof(direct);
    dir_buff[0].d_ino = tmp->dinode.di_addr[0];
    strcpy(dir_buff[0].d_name, ".");
    dir_buff[1].d_ino = tmp->dinode.di_addr[0];
    strcpy(dir_buff[1].d_name, "..");
    dir_buff[2].d_ino = 1;
    strcpy(dir_buff[2].d_name, "etc");
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) dir_buff, sizeof(direct) * 3);
    iput(tmp);

    tmp = iget(1);//etc目录
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 1;
    tmp->dinode.di_addr[0] = 1;
    tmp->dinode.di_size = 3 * sizeof(direct);
    dir_buff[0].d_ino = tmp->dinode.di_addr[0];
    strcpy(dir_buff[0].d_name, ".");
    dir_buff[1].d_ino = tmp->dinode.di_addr[0];
    strcpy(dir_buff[1].d_name, "..");
    dir_buff[2].d_ino = 2;
    strcpy(dir_buff[2].d_name, "password");
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) dir_buff, sizeof(direct) * 3);
    iput(tmp);

    tmp = iget(2);
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 2;
    iput(tmp);

    SuperBlock.s_isize = MAXINODE * DINODEBLK;
    SuperBlock.s_ninode = SuperBlock.s_isize - 3;
    SuperBlock.s_nfree = FILEBLK - 2;
    SuperBlock.s_fsize = FILEBLK;

    fs.seekp(0,ios::beg);
    fs.write((char*)&SuperBlock,sizeof(SuperBlock));
}