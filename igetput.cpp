//
// Created by 张译 on 2020/6/12.
//

#include "filesys.h"

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
    tmp->i_count = 1;
    tmp->nx = hinode[ind];
    hinode[ind] = tmp;

    return tmp;
}

int iput(inode *pinode) {
    int ind = pinode->i_ino % NHINO;

    inode *loc = hinode[ind];
    if (pinode->i_count > 1) {
        pinode->i_count--;
        return 0;
    } else {
        if (pinode->dinode.di_number != 0) {
            fs.seekp(GetDinodeloc(pinode->i_ino), ios::beg);
            dinode buff;
            buff = pinode->dinode;
            fs.seekp(GetDinodeloc(pinode->i_ino), ios::beg);
            fs.write((char *) &buff, sizeof(dinode));
            fs.flush();
        } else {
            filefree(pinode->dinode);
            ifree(pinode->i_ino);
        }

    }

    if (loc->i_ino == pinode->i_ino) {
        hinode[ind] = loc->nx;
        free(loc);
        return 1;
    }

    while (loc->nx) {
        inode *tmp = loc->nx;
        if (tmp->i_ino == pinode->i_ino) {
            loc->nx = tmp->nx;
            free(tmp);
            return 1;
        }
    }
    return  1;
}

