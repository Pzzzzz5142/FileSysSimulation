//
// Created by 张译 on 2020/6/21.
//

#include "filesys.h"

void cp(char *filename) {
    auto ii = opfl(curdir.user_id, filename, FREAD);
    memset(cpbuff, 0, sizeof(cpbuff));
    read(curdir.user_id, ii, cpbuff, sizeof(cpbuff));
    strcpy(cpname, filename);
    close(curdir.user_id, ii);
}

void pt() {
    if (cpp) {
        auto ii = create(curdir.user_id, cpname, FWRITE);
        write(curdir.user_id, ii, cpbuff, strlen(cpbuff) * sizeof(char));
        close(curdir.user_id, ii);
    } else
        ErrorHandling("You haven't copy a file");
}