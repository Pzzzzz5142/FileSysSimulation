//
// Created by 张译 on 2020/6/21.
//

#include "filesys.h"

void vi(char *filename) {
    auto ii = create(curdir.user_id, filename, FWRITE);
    system("vi /Users/pzzzzz/CLionProjects/UnixFileSysStimulator/buff");
    cout << "fi" << endl;
    string a;
    while (a == "") {
        ofstream file("/Users/pzzzzz/CLionProjects/UnixFileSysStimulator/buff");
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents(buffer.str());
        a = contents;
    }
    write(curdir.user_id, ii, (char *) a.c_str(), a.size() * sizeof(char));
    close(curdir.user_id, ii);
}