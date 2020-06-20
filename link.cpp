//李文雨 2020/6/20
//磁盘一级共享，同一文件可以在不同目录下存在，一处存储多处使用
//命令格式：share 当前目录下的文件/文件夹  目标路径
#include "filesys.h"

void share(char *filename, string targ) {
    if (targ.size() == 0||targ==".")
        ErrorHandling("No target path!");
    int i;
    //判断文件or文件夹是否存在
    int pos = name_is_exist(filename);
    if (pos == DIRNUM) {
        ErrorHandling("Fail!File or dictionary not exists!");
        return;
    }
    int d_ino_f = curdir.direct[pos].d_ino;//把该文件的i节点编号保存起来
    auto ii = iget(curdir.direct[pos].d_ino);
    if (ii->dinode.di_uid != curdir.user_id && ii->dinode.di_number == 1) {
        iput(ii);
        ErrorHandling("Permission denied!");
    }
    inode *tmp = curdir.inode;
    //切换到目标路径
    dir dir_tmp = curdir;
    chdir((char *) targ.c_str(), curdir);
    inode *tmp_d = curdir.inode;//目标目录的i节点

    //目标路径是否满了
    int res = curdir.size / sizeof(ddd);
    if (res == DIRNUM) {
        iput(curdir.inode);
        curdir = dir_tmp;
        curdir.inode = iget(curdir.direct[0].d_ino);
        ErrorHandling("Fail!Target dir is full!");
    }
    //目标路径是否有同名文件
    pos = name_is_exist(filename);
    if (pos != -1) {
        printf("File or dictionary already exists! \n");
    } else//没有同名文件
    {
        int pp = dir_alloc_name(filename);
        curdir.direct[pp].d_ino = d_ino_f;
        fs.seekp(DATASTART + BLOCKSIZ * tmp_d->dinode.di_addr[0], ios::beg);
        fs.write((char *) curdir.direct, curdir.size);
    }
    ii->dinode.di_number++;
    iput(ii);
    chdir(".", curdir);
    iput(curdir.inode);
    curdir = dir_tmp;
    curdir.inode = iget(curdir.direct[0].d_ino);
}