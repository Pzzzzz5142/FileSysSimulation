#include "filesys.h"


fstream fs("/Users/pzzzzz/CLionProjects/UnixFileSysStimulator/filesysdisk", ios::binary | ios::out | ios::in);
inode *hinode[NHINO]; //内存中inode的hash索引
dir curdir;
file sysopen_file[SYSOPENFILE];
user users[USERNUM];
SP SuperBlock;
map<string, int> mmp;
string cmd;

int main() {
    init();
    cmd = "";
    while (cmd != "y" && cmd != "n") {
        cout << "是否格式化磁盘？(y/n)：" << endl;
        cin >> cmd;
    }
    if (cmd == "y")
        format();
    install();
    while (cmd != "exit") {
        string usr;
        char psw[100000], c;
        int ind = 0;
        cout << "Username: ";
        cin >> usr;
        cout << "Password: ";
        getchar();
        while ((c = getchar()) != '\n') {
            if (c == '\b') {
                ind--;
                ind = max(0, ind);
            } else
                psw[ind++] = c;
        }
        psw[ind] = 0;
        try { login((char *) usr.c_str(), psw); }
        catch (string s) {
            cerr << s << endl;
            continue;
        }
        while (true) {
            show();
            getline(cin, cmd);
            string tcmd, tmp;
            vector<string> args;
            int cnt = 0;
            for (auto i:cmd) {
                if (i == ' ') {
                    if (cnt++ > 0)args.push_back(tmp);
                    tmp = "";
                } else if (cnt == 0)
                    tcmd += i;
                else
                    tmp += i;
            }
            args.push_back(tmp);
            try {
                if (tcmd == "ls") {
                    ls();
                } else if (tcmd == "open") {
                    int mode;
                    if (args[1] == "read")
                        mode = FREAD;
                    else if (args[1] == "write")
                        mode = FWRITE;
                    else {
                        ErrorHandling("Only support read and write mode.");
                    }
                    cout << opfl(curdir.user_id, (char *) args[0].c_str(), mode) << endl;
                } else if (tcmd == "create") {
                    auto i = create(curdir.user_id, (char *) args[0].c_str(), FWRITE);
                    mmp[args[0]] = i;
                    cout << i << endl;
                } else if (tcmd == "read") {
                    char buff[1000000];
                    read(curdir.user_id, atoi((char *) args[0].c_str()), buff, sizeof(buff));
                    cout << buff << endl;
                } else if (tcmd == "write") {
                    write(curdir.user_id, atoi((char *) args[0].c_str()), (char *) args[1].c_str(),
                          args[1].size() * sizeof(char));
                } else if (tcmd == "rm") {
                    del((char *) args[0].c_str());
                } else if (tcmd == "cd") {
                    chdir((char *) args[0].c_str(), curdir);
                } else if (tcmd == "close") {
                    close(curdir.user_id, atoi((char *) args[0].c_str()));
                }
                else if(tcmd=="mkdir")
                {
                    mkdir((char*)args[0].c_str());
                }
            } catch (string s) {
                cerr << s << endl;
            }
        }
    }
    return 0;
}
