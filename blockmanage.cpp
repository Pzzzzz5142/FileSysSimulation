//
// Created by 张译 on 2020/6/10.
//

#include "filesys.h"

int balloc() {
    if (SuperBlock.charge.ind < 0 || SuperBlock.charge.ind > GROUPNUM) {
        ErrorHandling("Bad SuperBlock");
    }

    auto &SuperInM = SuperBlock.charge;

    if (SuperInM.ind == 0) {
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
        SuperInM.stk[0] = num;
        SuperInM.ind = 0;
    } else {
        SuperInM.stk[++SuperInM.ind] = num;
    }
}

void filefree(dinode &d) {

    int blknum = d.di_size / BLOCKSIZ;
    if (blknum * BLOCKSIZ != d.di_size)
        blknum++;
    int free_num = 0;
    for (int i = 0; i < NADDR; i++) {
        if (free_num == blknum)
            break;
        if (i < NADDR - 3) {//无间址
            bfree(d.di_addr[i]);
            free_num++;
        } else if (i == NADDR - 3)//一次间址
        {
            free_num += undirect(d.di_addr[i], 0, blknum - free_num);
        } else if (i == NADDR - 2)//两次间址
        {
            free_num += undirect(d.di_addr[i], 1, blknum - free_num);
        } else//三次间址
            free_num += undirect(d.di_addr[i], 2, blknum - free_num);

    }
}