//
// Created by 张译 on 2020/6/9.
//

#ifndef UNIXFILESYSSTIMULATOR_FILESYS_H
#define UNIXFILESYSSTIMULATOR_FILESYS_H

#include <iostream>
#include <algorithm>
#include <string>
#include <memory.h>
#include <fstream>
#include <vector>
#include <map>
#include <string.h>
#include <sstream>

using namespace std;

const int BLOCKSIZ = 512; //每块的大小
#define SYSOPENFILE 40 //系统打开文件表的最大数目
#define DIRSIZ 14
#define PWDSIZ 12
#define PWDNUM 32
#define USEROPENFILE 20
#define NADDR 10
const int NHINO = 128;   //内存inode hash表长度
#define USERNUM 10
#define DINODESIZ 32
/*filsys*/
const int DINODEBLK = 32;
const int FILEBLK = 512;
#define NICFREE 50
#define NICINOD 50
#define DINODESTART  BLOCKSIZ
#define DATASTART (1+DINODEBLK)*BLOCKSIZ
#define GROUPNUM 50
/*di_node*/
#define DIEMPTY 00000
#define DIFILE 01000
#define DIDIR 02000
#define UDIREAD 00001   /*user*/
#define UDIWRITE 00002
#define UDIEXICUTE 00004
#define GDIREAD 00010    /* group*/
#define GDIWRITE 00020
#define GDIEXICUTE 00040
#define ODIREAD 00100     /*other*/
#define ODIWRITE 00200
#define ODIEXICUTE 00400
#define READ 1
#define WRITE 2
#define EXICUTE 3
#define DEFAULTMODE 00777
/*i_flag*/
#define IUPDATE 00002
/*s_fmod*/
#define SUPDATE 00001
/*f_flag*/
#define FREAD 00001
#define FWRITE 00002
#define FAPPEND 00004
/*error*/
#define DISKFULL 655535
/*fseek origin */
#define SEEK_SET 0
/*文件系统数据结构*/
struct dinode {
    unsigned short di_number;           /*关联文件数*/
    unsigned short di_mode;            /*存取权限*/
    unsigned short di_uid;
    unsigned long di_size;             /*文件大小*/
    unsigned int di_addr[NADDR];      /*物理块号*/
    int di_nx;
};

struct inode {
    struct inode *nx;
    unsigned int i_ino;                 /*磁盘i节点标志*/
    unsigned int i_count;               /*引用计数*/
    dinode dinode;

    inode() {
        nx = nullptr;
        i_count = 1;
        i_ino = 0;
    }
};

const int MAXINODE = BLOCKSIZ / sizeof(dinode);

struct direct {
    char d_name[DIRSIZ];
    int d_ino;

    direct operator=(const direct &a) {
        strcpy(d_name, a.d_name);
        d_ino = a.d_ino;
        return *this;
    }

    direct() {};

    direct(const direct &a) {
        strcpy(d_name, a.d_name);
        d_ino = a.d_ino;
    }
};

typedef direct ddd;

const int DIRNUM = BLOCKSIZ / sizeof(direct);
struct BlockCharge {
    int ind;
    int stk[GROUPNUM];
};
struct SP {
    BlockCharge charge;
    unsigned short s_isize;             /*i节点块数*/
    unsigned long s_fsize;             /*数据块数*/
    unsigned int s_nfree;              /*空闲块*/
    unsigned int s_ninode;          /*number of free inode in s_inode*/
    int ihead;
    char s_fmod;                   /*超级块修改标志*/
};

struct dir {
    vector<string> fa;
    direct direct[DIRNUM];
    inode *inode;
    int user_id;
    int size;                            /*当前目录大小*/
    dir operator=(const dir &a) {
        fa = a.fa;
        for (int i = 0; i < a.size / sizeof(ddd); i++)
            direct[i] = a.direct[i];
        inode = a.inode;
        user_id = a.user_id;
        size = a.size;
        return *this;
    }

    dir() {}

    dir(const dir &a) {
        fa = a.fa;
        for (int i = 0; i < a.size / sizeof(ddd); i++)
            direct[i] = a.direct[i];
        inode = a.inode;
        user_id = a.user_id;
        size = a.size;
    }
};

struct file {
    char f_flag;                          /*文件操作标志*/
    unsigned int f_count;                 /*引用计数*/
    inode *f_inode;                 /*指向内存i节点*/
    unsigned long f_off;              /*read/write character pointer*/
};
struct user {
    unsigned short u_default_mode;
    int u_uid;
    unsigned short u_ofile[USEROPENFILE];       /*用户打开文件表*/
    unsigned int open_num;
    /*system open file pointer number */
};
extern fstream fs;
extern inode *hinode[NHINO]; //内存中inode的hash索引
extern dir curdir;
extern file sysopen_file[SYSOPENFILE];
extern user users[USERNUM];
extern SP SuperBlock;
extern map<string, int> mmp;
extern char cpbuff[100000000], cpname[1000000];
extern int cpp;

template<class T>
void ReadABlock(int num, T &a) {
    fs.seekp(DATASTART + num * BLOCKSIZ, ios::beg);
    fs.read((char *) &a, sizeof(T));
}

template<class T>
void WriteABlock(int num, const T &a) {
    fs.seekp(DATASTART + num * BLOCKSIZ, ios::beg);
    fs.write((char *) &a, sizeof(T));
    fs.flush();
}


extern void ErrorHandling(const string &message);

extern inode *iget(int dinodeloc);

extern int iput(inode *pinode);

extern void ifree(int dinodeid);

extern int balloc();

extern int GetDinodeloc(int dinodeid);

extern void bfree(int num);

extern void format();

extern int name_is_exist(char *name);

extern int dir_alloc_name(char *name);

extern inode *ialloc();

extern void filefree(dinode &d);

extern void login(char *uid, char *passwd);

extern void logout(int uid);

extern int create(int user_id, char *name, int mode);

extern int opfl(int user_id, char *filename, int mode);

extern void close(unsigned int user_id, unsigned short cfd);

extern void vi(char *filename);

extern int read(int user, int fl, char *buff, int size);

extern int write(int user, int fl, char *buff, int size);

extern void del(char *filename);

extern void mkdir(char *name);

extern void chdir(char *path, dir &dr);

extern void ls();

extern vector<int> undirect(int loc, int lv, int lft);

extern void fileread(dinode &d, char *buff, int size, int off);

extern void filewrite(dinode &d, char *buff, int size, int off);

extern void install();

extern void init();

extern void show();

extern void move(char *filename, string targ);

extern void share(char *filename, string targ);

extern int halt();

extern void create_user(string name, string pass);

extern void cp(char *filename);

extern void pt();

#endif //UNIXFILESYSSTIMULATOR_FILESYS_H
