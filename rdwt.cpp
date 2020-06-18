//
// Created by 张译 on 2020/6/18.
//
#include "filesys.h"


int read(int user, int fl, char *buff, int size) {
    int off;
    int user_id = -1;
    for (int i = 0; i < USERNUM; i++) {
        if (users[i].u_uid == user) {
            user_id = i;
            break;
        }
    }
    if (user_id == -1) {
        ErrorHandling("This User is not logged in!");
    }

    auto &opfl = sysopen_file[users[user_id].u_ofile[fl]];

    inode *node = opfl.f_inode;
    if (opfl.f_flag != FREAD) {
        ErrorHandling("This file is not open for read!");
    }

    off = opfl.f_off;
    if ((off + size) < node->dinode.di_size) {
        size = node->dinode.di_size - off;
    }
    fileread(node->dinode, buff, size, off);
    off += size;
    return size;
}

int write(int user, int fl, char *buff, int size) {
    int off;
    int user_id = -1;
    for (int i = 0; i < USERNUM; i++) {
        if (users[i].u_uid == user) {
            user_id = i;
            break;
        }
    }
    if (user_id == -1) {
        ErrorHandling("This User is not logged in!");
    }

    auto &opfl = sysopen_file[users[user_id].u_ofile[fl]];

    inode *node = opfl.f_inode;
    if (opfl.f_flag != FWRITE && opfl.f_flag != FAPPEND) {
        ErrorHandling("This file is not open for write!");
    }

    if (opfl.f_flag == FWRITE) {
        off = 0;
        filefree(node->dinode);
    } else
        off = node->dinode.di_size;

    node->dinode.di_size = off + size;

    int blk = (size + BLOCKSIZ - 1) / BLOCKSIZ;
    int note = BLOCKSIZ / sizeof(int);
    int loc = 0;
    int off_in_blk = off % BLOCKSIZ, off_blk = off / BLOCKSIZ;
    int wt = 0;
    if (blk > SuperBlock.s_nfree)
        ErrorHandling("No Enough space!");
    for (int i = 0; i < blk; i++) {
        if (i < NADDR - 3) {
            if (i == off_blk) {
                if (opfl.f_flag == FWRITE && i == 0)
                    node->dinode.di_addr[0] = balloc();
                fs.seekp(DATASTART + BLOCKSIZ * node->dinode.di_addr[i] + off_in_blk, ios::beg);
                fs.write(buff + loc, min(BLOCKSIZ - off_in_blk, size - wt));
                loc += min(BLOCKSIZ - off_in_blk, size - wt);
                wt += min(BLOCKSIZ - off_in_blk, size - wt);
            } else if (i > off_blk) {
                node->dinode.di_addr[i] = balloc();
                fs.seekp(DATASTART + BLOCKSIZ * node->dinode.di_addr[i], ios::beg);
                fs.write(buff + loc, min(BLOCKSIZ - off_in_blk, size - wt));
                loc += min(BLOCKSIZ, size - wt);
                wt += min(BLOCKSIZ, size - wt);
            }
        } else if (i == NADDR - 3) {

        }
    }

}