//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void login(char *user, char *passwd) {
    int i, j;
    char buff[1000000], pwd[10000], usr[1000];
    inode *tmp = iget(3);
    fileread(tmp->dinode, buff, sizeof(buff), 0);
    int len = strlen(buff);
    int ud = -1, flg = 1, ind = 0, uid, uind = 0;
    for (int i = 0; i < len; i++) {
        if (buff[i] == '\t') {
            flg = 0;
            usr[uind] = 0;
            continue;
        } else if (buff[i] == '\n') {
            pwd[ind] = 0;
            ud++;
            if (strcmp(user, usr) == 0) {
                if (strcmp(pwd, passwd))
                    ErrorHandling("Invalid Password!");
                curdir.user_id = ud;
                int k = 1;
                for (int j = 0; j < USERNUM; j++) {
                    if (users[j].u_uid != -1) {
                        users[j].u_uid = uid;

                        k = 0;
                        break;
                    }
                }
                if (k) {
                    ErrorHandling("Too much users in the system!");
                }
                return;;
            }
            ud = 0;
            ind = 0;
        }
        if (flg == 1) {
            usr[uind++] = buff[i];
        } else {
            pwd[ind++] = buff[i];
        }
    }
    ErrorHandling("No such user!");
}

void logout(int uid)     /*logout*/
{
    int i, j, k, sys_no;
    inode *inode;
    for (k = 0; k < USERNUM; k++)
        if (uid == users[k].u_uid) {
            i = k;
            break;
        }
    if (k == USERNUM) {
        ErrorHandling("No such user!");
    }
    for (j = 0; j < USEROPENFILE; j++) {
        if (users[i].u_ofile[j] != SYSOPENFILE + 1) {   /* iput the inode free the sys_ofile and clear the user_ofile*/
            sys_no = users[i].u_ofile[j];
            users[i].u_uid = -1;
            inode = sysopen_file[sys_no].f_inode;
            iput(inode);
            sysopen_file[sys_no].f_count--;
            users[i].u_ofile[j] = SYSOPENFILE + 1;
        }
    }
}