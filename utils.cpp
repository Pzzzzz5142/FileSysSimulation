//
// Created by 张译 on 2020/6/10.
//

#include "filesys.h"

void ErrorHandling(const string &message) {
    cerr << message << endl;
    throw message;
}

int GetDinodeloc(int dinodeid) {
    return (dinodeid / MAXINODE + dinodeid % MAXINODE) * BLOCKSIZ + DINODESTART;
}

int undirect(int loc, int lv, int lft) {
    int buff[BLOCKSIZ / sizeof(unsigned int)];
    fs.seekp(BLOCKSIZ * loc + DATASTART, ios::beg);
    fs.read((char *) buff, sizeof(BlockCharge));
    int len = sizeof(buff) / sizeof(int);
    int tmp = 1;
    int res = 0;
    for (int i = 0; i < lv - 1; i++)
        tmp *= len;
    for (int i = 0; i < len && buff[i] != -1; i++) {
        if (lft <= 0)
            break;
        if (lv != 0) {
            res += undirect(buff[i], lv - 1, min(lft, tmp));
            lft -= tmp;
        } else {
            bfree(buff[i]);
            lft -= 1;
            res++;
        }
    }
    return res;
}