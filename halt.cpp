//
// Created by 张译 on 2020/6/20.
//
#include "filesys.h"

int halt() {
    struct inode *inode;
    int i, j;
    /*1. write back the current dir */
    chdir(".", curdir);
    iput(curdir.inode);
    /*2. free the u_ofile and sys_ofile and inode */
    for (i = 0; i < USERNUM; i++)
        if (users[i].u_uid != -1)
            for (j = 0; j < USEROPENFILE; j++)
                if (users[i].u_ofile[j] != SYSOPENFILE + 1) {
                    close(users[i].u_uid, users[i].u_ofile[j]);
                    users[i].u_ofile[j] = SYSOPENFILE + 1;
                }
/* 3. write back the filesys to the disk   */
    fs.seekp(0, ios::beg);
    fs.write((char *) &SuperBlock, sizeof(SP));
/*4. close the file system column*/
    fs.close();
/*5. say GOOD BYE to all the user */
    printf("\n Good Bye.See You Next Time.Please turn off the switch\n");
    exit(0);
}