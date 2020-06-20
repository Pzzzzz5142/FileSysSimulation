//
// Created by 张译 on 2020/6/13.
//

#include "filesys.h"

int create(int user_id, char *name, int mode) {
    int pos = name_is_exist(name);
    int user_ind = 0;

    for (user_ind = 0; user_ind < USERNUM; user_ind++) {
        if (users[user_ind].u_uid == user_id)
            break;
    }


    if (pos == -1) {
        int dir_num = dir_alloc_name(name);
        inode *tmp = ialloc();
        tmp->dinode.di_size = 0;
        tmp->dinode.di_mode = DIFILE;
        tmp->dinode.di_uid = user_id;
        tmp->dinode.di_number = 1;
        curdir.direct[dir_num].d_ino = tmp->i_ino;

        int syspos, userpos;
        for (syspos = 0; syspos < SYSOPENFILE; syspos++) {
            if (sysopen_file[syspos].f_count == 0)
                break;
        }
        if (syspos == SYSOPENFILE) {
            iput(tmp);
            ErrorHandling("System open too much file! ");
        }

        for (userpos = 0; userpos < USEROPENFILE; userpos++)
            if (users[user_ind].u_ofile[userpos] == SYSOPENFILE + 1)
                break;
        if (userpos == USEROPENFILE) {
            iput(tmp);
            ErrorHandling("User open too much file!");
        }
        users[user_ind].u_ofile[userpos] = syspos;
        users[user_ind].open_num++;
        sysopen_file[syspos].f_count = 1;
        sysopen_file[syspos].f_flag = FWRITE;
        sysopen_file[syspos].f_inode = tmp;
        sysopen_file[syspos].f_off = 0;

        return userpos;
    } else {
        printf("Warning! You are OVERWRITING the file!");
        inode *pre_inode = iget(curdir.direct[pos].d_ino);
        filefree(pre_inode->dinode);
        pre_inode->dinode.di_uid = user_id;
        pre_inode->dinode.di_size = 1;
        pre_inode->dinode.di_addr[0] = balloc();
        pre_inode->dinode.di_mode = DIFILE;
        for (int i = 0; i < SYSOPENFILE; i++)
            if (sysopen_file[i].f_inode == pre_inode)
                return i;
        int syspos, userpos;
        for (syspos = 0; syspos < SYSOPENFILE; syspos++) {
            if (sysopen_file[syspos].f_count == 0)
                break;
        }
        if (syspos == SYSOPENFILE) {
            iput(pre_inode);
            ErrorHandling("System open too much file! ");
        }

        for (userpos = 0; userpos < USEROPENFILE; userpos++)
            if (users[user_ind].u_ofile[userpos] == SYSOPENFILE + 1)
                break;
        if (userpos == USEROPENFILE) {
            iput(pre_inode);
            ErrorHandling("User open too much file!");
        }
        users[user_ind].u_ofile[userpos] = syspos;
        users[user_ind].open_num++;
        sysopen_file[syspos].f_count = 1;
        sysopen_file[syspos].f_flag = FWRITE;
        sysopen_file[syspos].f_inode = pre_inode;
        sysopen_file[syspos].f_off = 0;
        return userpos;
    }
}

void create_user(string name, string pass) {
    if (curdir.user_id != 0)
        ErrorHandling("Permission Denied!");
    char buff[100000];
    dir tmp = curdir;
    chdir("/etc", curdir);
    auto fl = opfl(curdir.user_id, "password", FREAD);
    read(curdir.user_id, fl, buff, sizeof(buff));
    close(curdir.user_id, fl);
    string a = buff;
    a += name + "\t" + pass + "\n";
    fl = opfl(curdir.user_id, "password", FWRITE);
    write(curdir.user_id, fl, (char *) a.c_str(), a.size() * sizeof(char));
    close(curdir.user_id, fl);
    chdir("/User", curdir);
    mkdir((char *) name.c_str());
    chdir(".", curdir);
    iput(curdir.inode);
    curdir = tmp;
    curdir.inode = iget(curdir.direct[0].d_ino);
}