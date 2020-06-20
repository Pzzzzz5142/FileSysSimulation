//
// Created by 张译 on 2020/6/10.
//

#include "filesys.h"

void ErrorHandling(const string &message) {
    cerr << message << endl;
    throw message;
}

int GetDinodeloc(int dinodeid) {
    return (dinodeid / MAXINODE) * BLOCKSIZ + DINODESTART+(dinodeid%MAXINODE)*sizeof(dinode);
}

vector<int> undirect(int loc, int lv, int lft) {
    int buff[BLOCKSIZ / sizeof(unsigned int)];
    fs.seekp(BLOCKSIZ * loc + DATASTART, ios::beg);
    fs.read((char *) buff, sizeof(buff));
    int len = sizeof(buff) / sizeof(int);
    int tmp = 1;
    vector<int> res;
    for (int i = 0; i < lv - 1; i++)
        tmp *= len;
    for (int i = 0; i < len && lft > 0; i++, lft -= tmp) {
        if (lv != 0) {
             vector<int>tt= undirect(buff[i], lv - 1, min(lft, tmp));
             for(auto x:tt)
                 res.push_back(x);
        } else {
            res.push_back(i);
        }
    }
    return res;
}

void init()
{
    mmp["ls"]=0;
    mmp["open"]=1;
    mmp["close"]=2;
    mmp["create"]=3;
    mmp["read"]=4;
    mmp["write"]=5;
    mmp["delete"]=6;
    mmp["mkdir"]=7;
    mmp["cd"]=8;
    mmp["cat"]=9;
}