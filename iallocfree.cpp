//
// Created by 张译 on 2020/6/12.
//

#include "filesys.h"

inode *ialloc() {
    if (SuperBlock.s_nfree == 0) {
        ErrorHandling("No Enough inode space");
    }

    inode *tmp = iget(SuperBlock.ihead);
    SuperBlock.s_ninode--;
    SuperBlock.ihead = tmp->dinode.di_nx;
    return tmp;
}

void ifree(int dinodeid) {
    fs.seekp(GetDinodeloc(dinodeid), ios::beg);
    dinode buff;
    memset(&buff,0,sizeof(buff));
    buff.di_nx = SuperBlock.ihead;

    fs.write((char *) &buff, sizeof(dinode));
    SuperBlock.ihead = dinodeid;
    SuperBlock.s_ninode++;
}