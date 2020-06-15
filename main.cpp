#include "filesys.h"

fstream fs("/Users/pzzzzz/CLionProjects/UnixFileSysStimulator/filesysdisk", ios::binary | ios::out | ios::in);
inode *hinode[NHINO]; //内存中inode的hash索引
dir curdir;

int main() {
    std::cout << "Hello, World!" << std::endl;
    format();
    cout << sizeof(SuperBlock);
    return 0;
}
