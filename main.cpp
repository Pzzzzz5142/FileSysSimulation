#include "filesys.h"


fstream fs("/Users/pzzzzz/CLionProjects/UnixFileSysStimulator/filesysdisk", ios::binary | ios::out | ios::in);
inode *hinode[NHINO]; //内存中inode的hash索引
dir curdir;
file sysopen_file[SYSOPENFILE];
user users[USERNUM];
SP SuperBlock;
char cpbuff[100000000], cpname[1000000];
int cpp;
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
        cpp = 0;
        char psw[100000], c;
        int ind = 0;
        cout << "Username: ";
        cin >> usr;
        cout << "Password: ";
        cin >> psw;
        try { login((char *) usr.c_str(), psw); }
        catch (string s) {
            continue;
        }
        cin.get();
        while (true) {
            show();

            getline(cin, cmd);
            string tcmd, tmp, orr;
            vector<string> args;
            int cnt = 0;
            for (auto i:cmd) {
                if (i == ' ') {
                    if (cnt++ > 0)args.push_back(tmp);
                    if (cnt > 2)
                        orr += i;
                    tmp = "";
                } else if (cnt == 0)
                    tcmd += i;
                else {
                    tmp += i;
                    if (cnt > 1)
                        orr += i;
                }
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
                    write(curdir.user_id, atoi((char *) args[0].c_str()), (char *) orr.c_str(),
                          orr.size() * sizeof(char));
                } else if (tcmd == "rm") {
                    del((char *) args[0].c_str());
                } else if (tcmd == "cd") {
                    chdir((char *) args[0].c_str(), curdir);
                } else if (tcmd == "close") {
                    close(curdir.user_id, atoi((char *) args[0].c_str()));
                } else if (tcmd == "mkdir") {
                    mkdir((char *) args[0].c_str());
                } else if (tcmd == "cat") {
                    auto aa = opfl(curdir.user_id, (char *) args[0].c_str(), FREAD);
                    char buff[1000000];
                    read(curdir.user_id, aa, buff, sizeof(buff));
                    cout << buff << endl;
                    close(curdir.user_id, aa);
                } else if (tcmd == "mv") {
                    move((char *) args[0].c_str(), args[1]);
                } else if (tcmd == "ln") {
                    share((char *) args[0].c_str(), args[1]);
                } else if (tcmd == "create_user") {
                    create_user(args[0], args[1]);
                } else if (tcmd == "cp") {
                    cpp = 1;
                    cp((char *) args[0].c_str());
                } else if (tcmd == "pt") {
                    pt();
                } else if (tcmd == "logout") {
                    logout(curdir.user_id);
                    break;
                }
            } catch (string s) {
            }
        }
        while (cmd != "y" && cmd != "n") {
            cout << "退出吗？(y/n)：";
            cin >> cmd;
        }
        if (cmd == "y")
            break;
    }
    halt();
    return 0;
}
