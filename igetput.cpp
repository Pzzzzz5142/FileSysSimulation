//
// Created by 张译 on 2020/6/12.
//

#include "filesys.h"

void undirect(int loc, int lv) {
    int buff[BLOCKSIZ / sizeof(unsigned int)];
    fs.seekp(BLOCKSIZ * loc + DATASTART, ios::beg);
    fs.read((char *) buff, sizeof(BlockCharge));
    int len = sizeof(buff) / sizeof(int);
    for (int i = 0; i < len && buff[i] != -1; i++) {
        if (lv != 0)
            undirect(buff[i], lv - 1);
        else
            bfree(buff[i]);
    }
}

inode *iget(int dinodeloc) {
    bool exist = false;
    int ind = dinodeloc % NHINO;
    inode *loc = hinode[ind];

    while (loc) {
        if (loc->i_ino == dinodeloc) {
            exist = true;
            loc->i_count += 1;
            break;
        }
        loc = loc->nx;
    }

    if (exist) {
        return loc;
    }

    dinode buff;
    fs.seekp(GetDinodeloc(dinodeloc), ios::beg);
    fs.read((char *) &buff, sizeof(buff));

    inode *tmp = new inode();
    tmp->dinode = buff;
    tmp->i_ino = dinodeloc;
    if (hinode[ind]) {
        tmp->nx = hinode[ind];
        hinode[ind] = tmp;
    }
    return tmp;
}

void iput(inode *pinode) {
    int ind = pinode->i_ino % NHINO;

    inode *loc = hinode[ind];
    if (pinode->i_count > 1) {
        pinode->i_count--;
        return;
    } else {
        if (pinode->dinode.di_number != 0) {
            fs.seekp(GetDinodeloc(pinode->i_ino), ios::beg);
            fs.write((char *) &(pinode->dinode), sizeof(dinode));
        } else {
            int blknum = pinode->dinode.di_size / BLOCKSIZ;
            if (blknum * BLOCKSIZ != pinode->dinode.di_size)
                blknum++;
            for (int i = 0; i < blknum; i++) {
                if (i < NADDR - 3) {//无间址
                    bfree(pinode->dinode.di_addr[i]);
                } else if (i == NADDR - 3)//一次间址
                {
                    undirect(pinode->dinode.di_addr[i], 0);
                } else if (i == NADDR - 2)//两次间址
                {
                    undirect(pinode->dinode.di_addr[i], 1);
                } else//三次间址
                    undirect(pinode->dinode.di_addr[i], 2);
            }
            ifree(pinode->i_ino);
        }

    }

    if (loc->i_ino == pinode->i_ino) {
        hinode[ind] = loc->nx;
        free(loc);
        return;
    }

    while (loc->nx) {
        inode *tmp = loc->nx;
        if (tmp->i_ino == pinode->i_ino) {
            loc->nx = tmp->nx;
            free(tmp);
            return;
        }
    }
}

