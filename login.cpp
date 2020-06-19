//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void login(char* user, char *passwd) {
    int i, j;
    char buff[1000000], pwd[10000],usr[1000];
    inode *tmp = iget(2);
    fileread(tmp->dinode, buff, sizeof(buff), 0);
    int len = strlen(buff);
    int ud = 0, flg = 1, ind = 0,uid,uind=0;
    for (int i = 0; i < len; i++) {
        if (buff[i] == '\t') {
            flg = 0;
            usr[uind]=0;
            continue;
        } else if (buff[i] == '\n') {
            pwd[ind] = 0;
            if (strcmp(user,usr)==0) {
                if(strcmp(pwd,passwd))
                    ErrorHandling("Invalid Password!");
                curdir.user_id = ud;
                int k=1;
                for(int j=0;j<USERNUM;j++)
                {
                    if(users[j].u_uid!=-1)
                    {
                        users[j].u_uid=uid;
                        k=0;
                        break;
                    }
                }
                if(k)
                {
                    ErrorHandling("Too much users in the system!");
                }
                return;;
            }
            ud = 0;
            ind=0;
        }
        if (flg == 1) {
            ud += buff[i] - '0';
            ud *= 10;
        } else {
            pwd[ind++]=buff[i];
        }
    }
    ErrorHandling("No such user!");
}