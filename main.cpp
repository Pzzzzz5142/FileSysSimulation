#include "filesys.h"

fstream fs("/Users/pzzzzz/CLionProjects/UnixFileSysStimulator/filesysdisk", ios::binary | ios::out | ios::in);
inode *hinode[NHINO]; //内存中inode的hash索引
dir curdir;
file sysopen_file[SYSOPENFILE];
user users[USERNUM];
SP SuperBlock;

int main() {
    std::cout << "Hello, World!" << std::endl;
    format();
    cout << sizeof(SuperBlock);
    curdir.size = 3;
    curdir.direct[0].d_ino = 0;
    curdir.direct[1].d_ino = 0;
    curdir.direct[2].d_ino = 1;
    for (int i = 3; i < DIRNUM; i++) {
        curdir.direct[i].d_ino = -1;
    }char buff[10000];
    for (int i = 0; i < USEROPENFILE; i++)
        users[0].u_ofile[i] = SYSOPENFILE + 1;
    try {
        auto cre = create(0, "hello", FWRITE);
        auto wt = write(0, cre, "Hello World", sizeof("Hello World"));
        close(0, cre);
        auto op = opfl(0, "hello", FREAD);

        read(0, op, buff, 100000);
    } catch (string s) {
        cout << s << endl;
    }

    fs.close();
    return 0;
}
