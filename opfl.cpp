//
// Created by 张译 on 2020/6/15.
//

#include "filesys.h"

int opfl(int user_id,char* filename,int mode)
{
    int dirid=name_is_exist(filename);
    if(dirid==-1)
    {
        ErrorHandling("File not exist!");
    }
    inode* node=iget(curdir.direct[dirid].d_ino);


}