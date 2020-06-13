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

