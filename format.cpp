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
    memset(hinode, 0, sizeof(hinode));

    SuperBlock.charge.ind = 0;
    SuperBlock.charge.stk[0] = -1;
    for (int i = 4; i < FILEBLK; i++)//初始化成组链接法，0为根目录，1为etc目录
    {
        bfree(i);
    }

    SuperBlock.ihead = -1;
    for (int i = 4; i < MAXINODE * DINODEBLK; i++) {
        ifree(i);
    }

    inode *tmp = iget(0);//最根的目录
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 1;
    tmp->dinode.di_uid = 0;
    tmp->dinode.di_addr[0] = 0;
    tmp->dinode.di_size = 4 * sizeof(direct);
    dir_buff[0].d_ino = tmp->dinode.di_addr[0];
    strcpy(dir_buff[0].d_name, ".");
    dir_buff[1].d_ino = 0;
    strcpy(dir_buff[1].d_name, "..");
    dir_buff[2].d_ino = 0;
    strcpy(dir_buff[2].d_name, "etc");
    dir_buff[2].d_ino = 1;
    strcpy(dir_buff[3].d_name, "User");
    dir_buff[3].d_ino = 2;
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) dir_buff, sizeof(direct) * 4);
    iput(tmp);

    tmp = iget(1);//etc目录
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 1;
    tmp->dinode.di_uid = 0;
    tmp->dinode.di_addr[0] = 1;
    tmp->dinode.di_size = 3 * sizeof(direct);
    dir_buff[0].d_ino = 1;
    strcpy(dir_buff[0].d_name, ".");
    dir_buff[1].d_ino = 0;
    strcpy(dir_buff[1].d_name, "..");
    dir_buff[2].d_ino = 3;
    strcpy(dir_buff[2].d_name, "password");
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) dir_buff, sizeof(direct) * 3);
    iput(tmp);

    tmp = iget(2);//User目录
    tmp->dinode.di_mode = DIDIR;
    tmp->dinode.di_number = 1;
    tmp->dinode.di_addr[0] = 2;
    tmp->dinode.di_size = 2 * sizeof(direct);
    dir_buff[0].d_ino = 2;
    strcpy(dir_buff[0].d_name, ".");
    dir_buff[1].d_ino = 0;
    tmp->dinode.di_uid = 0;
    strcpy(dir_buff[1].d_name, "..");
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) dir_buff, sizeof(direct) * 2);
    iput(tmp);

    tmp = iget(3);
    tmp->dinode.di_mode = DIFILE;
    tmp->dinode.di_number = 1;
    tmp->dinode.di_uid = 0;
    strcpy(buff, "root\tpassword\n");
    tmp->dinode.di_size = strlen(buff) * sizeof(char);
    tmp->dinode.di_addr[0] = 3;
    fs.seekp(DATASTART + tmp->dinode.di_addr[0] * BLOCKSIZ, ios::beg);
    fs.write(buff, sizeof(char) * (strlen(buff) + 1));
    iput(tmp);

    SuperBlock.s_isize = MAXINODE * DINODEBLK;
    SuperBlock.s_ninode = SuperBlock.s_isize - 3;
    SuperBlock.s_nfree = FILEBLK - 2;
    SuperBlock.s_fsize = FILEBLK;

    for (int i = 0; i < SYSOPENFILE; i++) {
        sysopen_file[i].f_inode = nullptr;
    }

    fs.seekp(0, ios::beg);
    fs.write((char *) &SuperBlock, sizeof(SuperBlock));
    fs.flush();
}