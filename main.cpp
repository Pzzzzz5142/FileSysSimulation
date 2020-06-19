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
    install();
    login(0,"password");
    char buff[10000] = "Hello World alasdhfklklkajhdsflkjahsldfkhajsfdlsdjmmasldkfalsdhfkljashdflkjhasdflkjhaslkfdhklhadfkjhalksdhjflkahsdflkahjsdflf;laksdjf;alskjdfkjabsdflkajchiewruopisdfalkshdfkbvljkhdfjhpoeiurqwoiaksjdhfakbv.asdfhsf;lkahsdfjhakjsdfhlaksjdhflkasdjhfiopuerlajshdfklajhsfdklajhsdfkljahsdkfjhaksdjhfkashdfkjhasfdkhsakdfhkasjdhfkjshadfkjasfdkshdflkajhsdflkjahsdlkfjhalskdjfhlakjshdfklajshdfnambcxvzm,nxbcvlaksjdfhlkasjhdfiquweyralksjdfhlaksjhdflaksjhmnbxvm,znxbcvjkshdflkasjdhfasknbm,znbvlaksjhdfkajshriqwufeaskdflkasbvmnasbflkajhsdfkljasfsf";
    for(int i=strlen(buff);i<2048;i++){
        buff[i]='i';
        buff[i+1]='\0';
    }
    cout<<strlen(buff)*sizeof(char)<<endl;
    try {
        auto cre = create(0, "hello", FWRITE);
        write(0, cre, buff, sizeof(char)*strlen(buff));
        close(0, cre);
        auto op = opfl(0, "hello", FREAD);
        read(0, op, buff, 100000);
        cout << buff << endl;
        cout<<strlen(buff)<<endl;
    } catch (string s) {
        cout << s << endl;
    }

    //fs.close();
    return 0;
}
