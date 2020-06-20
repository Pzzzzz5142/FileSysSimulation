//李文雨 2020/6/20
//磁盘一级共享，同一文件可以在不同目录下存在，一处存储多处使用
//命令格式：share 当前目录下的文件/文件夹  目标路径
#include "filesys.h"

void share(char *filename, string targ) {
    int i;
    if(targ.size()==0)
        ErrorHandling("No target path!");
    //判断文件or文件夹是否存在
    int pos = name_is_exist(filename);
    if (pos == -1) {
        ErrorHandling("Fail!File or dictionary not exists!");
    }
    int d_ino_f = curdir.direct[pos].d_ino;//把该文件的i节点编号保存起来
    inode *tem_file = iget(curdir.direct[pos].d_ino);//该文件的i节点

    //切换到目标路径
    dir dir_tmp = curdir;
    chdir((char *) targ.c_str(), curdir);
    inode *tmp_d = iget(curdir.direct[0].d_ino);//目标目录的i节点

    //目标路径是否满了
    int res = curdir.size / sizeof(ddd);
    if (res == DIRNUM) {
        iput(tmp_d);
        iput(tem_file);
        ErrorHandling("Fail!Current dir is full!");
    }
    //目标路径是否有同名文件
    pos = name_is_exist(filename);
    if (pos != -1) {
        iput(tmp_d);
        iput(tem_file);
        ErrorHandling("File or dictionary already exists, link failed.");
    } else//没有同名文件
    {
        int pp = dir_alloc_name(filename);
        curdir.direct[pp].d_ino = d_ino_f;
        fs.seekp(DATASTART + BLOCKSIZ * tmp_d->dinode.di_addr[0], ios::beg);
        fs.write((char *) curdir.direct, sizeof(direct) * curdir.size);
    }
    tem_file->dinode.di_number++;//共享成功，关联文件数加一
    iput(tem_file);
    iput(tmp_d);
    chdir(".", curdir);
    curdir = dir_tmp;
}