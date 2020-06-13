//
// Created by 张译 on 2020/6/12.
//
#include "filesys.h"

void format() {
    char buff[(FILEBLK + DINODEBLK + 1) * BLOCKSIZ];
    memset(buff, 0, sizeof(buff));
    fs.seekp(0, ios::beg);
    fs.write(buff, sizeof(buff));
}