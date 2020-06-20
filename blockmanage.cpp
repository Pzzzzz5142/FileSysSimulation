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
    for (int i = 0; i < NADDR && i < blknum; i++) {
        if (i < NADDR - 3) {//无间址
            bfree(d.di_addr[i]);
            free_num++;
        } else if (i == NADDR - 3)//一次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 0, blknum - free_num);
            free_num += tmp.size();
            for (auto x:tmp)
                bfree(x);
        } else if (i == NADDR - 2)//两次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 1, blknum - free_num);
            free_num += tmp.size();
            for (auto x:tmp)
                bfree(x);
        } else//三次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 2, blknum - free_num);
            free_num += tmp.size();
            for (auto x:tmp)
                bfree(x);
        }
    }
    d.di_size = 0;
}

void fileread(dinode &d, char *buff, int size, int off) {
    int blknum = d.di_size / BLOCKSIZ;
    if (blknum * BLOCKSIZ != d.di_size)
        blknum++;
    int read_num = 0;
    int off_blk = off / BLOCKSIZ, off_in_blk = off % BLOCKSIZ;
    int loc = 0;
    for (int i = 0; i < NADDR && size > 0; i++) {
        if (i < NADDR - 3) {//无间址
            if (read_num == off_blk) {
                fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i] + off_in_blk, ios::beg);
                fs.read(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                loc += BLOCKSIZ - off_in_blk;
                size -= min(BLOCKSIZ - off_in_blk, size);
                read_num++;
            } else if (read_num > off_blk) {
                fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i], ios::beg);
                fs.read(buff + loc, min(size, BLOCKSIZ));
                loc += BLOCKSIZ - off_in_blk;
                size -= BLOCKSIZ;
                read_num++;
            }
        } else if (i == NADDR - 3)//一次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 0, blknum - read_num);
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i] + off_in_blk, ios::beg);
                    fs.read(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                    read_num++;
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i], ios::beg);
                    fs.read(buff + loc, min(size, BLOCKSIZ));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= BLOCKSIZ;
                    read_num++;
                }
            }
        } else if (i == NADDR - 2)//两次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 1, blknum - read_num);
            read_num += tmp.size();
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i] + off_in_blk, ios::beg);
                    fs.read(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                    read_num++;
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i], ios::beg);
                    fs.read(buff + loc, min(size, BLOCKSIZ));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= BLOCKSIZ;
                    read_num++;
                }
            }
        } else//三次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 2, blknum - read_num);
            read_num += tmp.size();
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i] + off_in_blk, ios::beg);
                    fs.read(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                    read_num++;
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i], ios::beg);
                    fs.read(buff + loc, min(size, BLOCKSIZ));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= BLOCKSIZ;
                    read_num++;
                }
            }
        }
    }
}

void filewrite(dinode &d, char *buff, int size, int off) {
    int blknum = d.di_size / BLOCKSIZ;
    filefree(d);
    if (blknum * BLOCKSIZ != d.di_size)
        blknum++;
    int read_num = 0;
    int off_blk = off / BLOCKSIZ, off_in_blk = off % BLOCKSIZ;
    int loc = 0;
    for (int i = 0; i < NADDR && size > 0; i++) {
        if (i < NADDR - 3) {//无间址
            read_num++;
            if (read_num == off_blk) {
                d.di_addr[i] = balloc();
                fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i] + off_in_blk, ios::beg);
                fs.write(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                fs.flush();
                loc += BLOCKSIZ - off_in_blk;
                size -= min(BLOCKSIZ - off_in_blk, size);
            } else if (read_num > off_blk) {
                if (i > blknum)
                    d.di_addr[i] = balloc();
                fs.seekp(DATASTART + BLOCKSIZ * d.di_addr[i], ios::beg);
                fs.write(buff + loc, min(size, BLOCKSIZ));
                fs.flush();
                loc += BLOCKSIZ - off_in_blk;
                size -= BLOCKSIZ;
            }
        } else if (i == NADDR - 3)//一次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 0, blknum - read_num);
            read_num += tmp.size();
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x + off_in_blk, ios::beg);
                    fs.write(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    fs.flush();
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x, ios::beg);
                    fs.write(buff + loc, min(size, BLOCKSIZ));
                    fs.flush();
                    loc += BLOCKSIZ;
                    size -= BLOCKSIZ;
                }
            }
        } else if (i == NADDR - 2)//两次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 1, blknum - read_num);
            read_num += tmp.size();
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x + off_in_blk, ios::beg);
                    fs.write(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    fs.flush();
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x, ios::beg);
                    fs.write(buff + loc, min(size, BLOCKSIZ));
                    fs.flush();
                    loc += BLOCKSIZ;
                    size -= BLOCKSIZ;
                }
            }
        } else//三次间址
        {
            vector<int> tmp = undirect(d.di_addr[i], 2, blknum - read_num);
            read_num += tmp.size();
            for (auto x:tmp) {
                if (size < 0)
                    break;
                if (read_num == off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x + off_in_blk, ios::beg);
                    fs.read(buff + loc, min(BLOCKSIZ - off_in_blk, size));
                    loc += BLOCKSIZ - off_in_blk;
                    size -= min(BLOCKSIZ - off_in_blk, size);
                } else if (read_num > off_blk) {
                    fs.seekp(DATASTART + BLOCKSIZ * x, ios::beg);
                    fs.read(buff + loc, min(size, BLOCKSIZ));
                    loc += BLOCKSIZ;
                    size -= BLOCKSIZ;
                }
            }
        }
    }
}