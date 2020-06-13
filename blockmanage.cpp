//
// Created by 张译 on 2020/6/10.
//

#include "filesys.h"

int balloc() {
    if (SuperBlock.charge.ind < 0 || SuperBlock.charge.ind > GROUPNUM) {
        ErrorHandling("Bad SuperBlock")
    }

    auto &SuperInM = SuperBlock.charge;

    if (SuperInM.ind == 1) {
        int alloc = SuperInM.stk[SuperInM.ind];
        SuperBlock.s_nfree--;
        ReadABlock<BlockCharge>(SuperInM.stk[SuperInM.ind], SuperInM);
        return alloc;
    } else {
        SuperBlock.s_nfree--;
        return SuperInM.stk[SuperInM.ind--];
    }
}

void bfree(int num) {
    auto &SuperInM = SuperBlock.charge;
    if (SuperInM.ind == GROUPNUM) {
        WriteABlock<BlockCharge>(num, SuperInM);
        SuperInM.ind = -1;
    } else {
        SuperInM.stk[++SuperInM.ind] = num;
    }
}