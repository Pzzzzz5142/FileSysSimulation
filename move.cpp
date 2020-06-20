//李文雨 2020/6/19
//把文件/文件夹从一个目录下移动到另一个目录下
//命令格式：move 当前目录下的文件/文件夹  目标路径
#include "filesys.h"

void move(char *filename, string targ) {
    if(targ.size()==0)
        ErrorHandling("No target path!");
    int i;
    //判断文件or文件夹是否存在
    int pos = name_is_exist(filename);
    if (pos == -1) {
        ErrorHandling("Fail!File or dictionary not exists!");
        return;
    }
    int d_ino_f = curdir.direct[pos].d_ino;//把该文件的i节点编号保存起来
    //把源文件所在的目录里面的源文件记录删除，写回磁盘
    inode *tmp = iget(curdir.direct[0].d_ino);//0是当前目录的d_ino
    for (int j = pos; j < curdir.size / sizeof(ddd) - 1; j++) {
        curdir.direct[j] = curdir.direct[j + 1];
    }
    ddd a;
    cout << sizeof(ddd) << endl;
    cout << sizeof(direct) << endl;
    curdir.size -= sizeof(ddd);
    tmp->dinode.di_size -= sizeof(ddd);
    fs.seekp(DATASTART + BLOCKSIZ * tmp->dinode.di_addr[0], ios::beg);
    fs.write((char *) curdir.direct, curdir.size);
    iput(tmp);

    //切换到目标路径
    dir dir_tmp = curdir;
    chdir((char *) targ.c_str(), curdir);
    inode *tmp_d = iget(curdir.direct[0].d_ino);//目标目录的i节点

    //目标路径是否满了
    int res = curdir.size / sizeof(ddd);
    if (res == DIRNUM) {
        iput(tmp_d);
        ErrorHandling("Fail!Current dir is full!");
    }
    //目标路径是否有同名文件
    pos = name_is_exist(filename);
    if (pos != -1) {
        printf("File or dictionary already exists! Renaming!v\n");
        printf("Rename:");
        int ind = 0;
        string a = filename, tmp;
        while (pos != -1) {
            tmp = a + to_string(ind);
            pos = name_is_exist((char *) tmp.c_str());
            ind++;
        }
        cout << tmp << endl;
        int pp = dir_alloc_name((char *) tmp.c_str());
        curdir.direct[pp].d_ino = d_ino_f;
        fs.seekp(DATASTART + BLOCKSIZ * tmp_d->dinode.di_addr[0], ios::beg);
        fs.write((char *) curdir.direct, sizeof(direct) * curdir.size);
        iput(tmp_d);

    } else//没有同名文件
    {
        int pp = dir_alloc_name(filename);
        curdir.direct[pp].d_ino = d_ino_f;
        fs.seekp(DATASTART + BLOCKSIZ * tmp_d->dinode.di_addr[0], ios::beg);
        fs.write((char *) curdir.direct, sizeof(direct) * curdir.size);
        iput(tmp_d);
    }
    iput(tmp_d);
    chdir(".", curdir);
    curdir = dir_tmp;
}