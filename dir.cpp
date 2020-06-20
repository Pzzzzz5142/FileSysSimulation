//
// Created by 张译 on 2020/6/19.
//

#include "filesys.h"

void ls() {
    cout << "Name\tType" << endl;
    for (int i = 0; i < curdir.size / sizeof(direct); i++) {
        auto tmp = iget(curdir.direct[i].d_ino);
        if (tmp->dinode.di_mode == DIFILE)
            cout << curdir.direct[i].d_name << "\t\tFile" << endl;
        else
            cout << curdir.direct[i].d_name << "\t\tDir" << endl;
        iput(tmp);
    }
}

void show() {
    for (auto i:curdir.fa) {
        cout << '\\' << i;
    }
    if (curdir.fa.size() == 0)
        cout << "根目录";
    cout << ':';
}

void SplitString(const string &s, vector<string> &v, const string &c) {
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (string::npos != pos2) {
        if (s.substr(pos1, pos2 - pos1) == "")
            continue;
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

dir find_path(vector<string> chpath, dir tt) {
    dir new_dir = tt;
    for (int i = 0; i < tt.size / sizeof(direct); i++) {
        if (tt.direct[i].d_name == chpath[0]) {
            inode *temp_inode = iget(tt.direct[i].d_ino);
            if (temp_inode->dinode.di_mode != DIDIR) {
                iput(temp_inode);
                continue;
            } else {
                iput(tt.inode);
                vector<string> tmp;
                for (int i = 1; i < chpath.size(); i++)
                    tmp.push_back(chpath[i]);
                if (temp_inode == tt.inode)
                    if (chpath.size() > 1)
                        return find_path(tmp, tt);
                    else
                        return tt;
                fileread(temp_inode->dinode, (char *) new_dir.direct, temp_inode->dinode.di_size, 0);
                new_dir.inode = temp_inode;
                new_dir.size = temp_inode->dinode.di_size;
                if (chpath[0] == "..")
                    new_dir.fa.pop_back();
                else if (chpath[0] != ".")
                    new_dir.fa.push_back(chpath[0]);
                if (chpath.size() > 1)
                    return find_path(tmp, new_dir);
                else
                    return new_dir;
            }
        }
    }
    ErrorHandling("Path is not correct!");
}

void chdir(char *path, dir &dr) {
    int i, j, ino, k, l;
    inode *temp_inode;
    inode *cur_path_inode;

    vector<string> chdirp;

    inode *inode;
    unsigned short block;
    int cur_ino;
    string ph = path;
    filefree(dr.inode->dinode);
    dr.inode->dinode.di_size = dr.size;
    int ind = 0, size = min(BLOCKSIZ, dr.size), ii = 0, x = dr.size;
    while (x) {
        block = balloc();
        dr.inode->dinode.di_addr[ii++] = block;
        fs.seekp(DATASTART + block * BLOCKSIZ, ios::beg);
        fs.write((char *) dr.direct + ind, size);
        ind += size / sizeof(direct);
        x -= size;
        size = min(BLOCKSIZ, x);
    }
    dir new_dir;
    if (ph[0] != '/') {
        SplitString(ph, chdirp, "/");
        dr = find_path(chdirp, dr);
    } else {
        string tmp;
        for (int i = 1; i < ph.size(); i++)
            tmp += ph[i];
        SplitString(tmp, chdirp, "/");
        iput(dr.inode);
        auto *cur_path_inode = iget(0);
        dr.size = cur_path_inode->dinode.di_size;
        dr.inode = cur_path_inode;
        fileread(cur_path_inode->dinode, (char *) dr.direct, dr.size, 0);
        for (int i = dr.size / sizeof(direct); i < DIRNUM; i++)
            dr.direct[i].d_ino = -1;
        dr.fa.clear();
        dr = find_path(chdirp, dr);
    }
}

void mkdir(char *name) {
    int dirid, dirpos;
    inode *inode;
    direct buff[5];
    unsigned int block;


    memset(buff, 0, sizeof(buff));
    dirid = name_is_exist(name);

    if (dirid != -1) {
        ErrorHandling("There is something already exist.");
    }


    dirpos = dir_alloc_name(name);
    inode = ialloc();

//  inode->i_ino=dirid;
    curdir.direct[dirpos].d_ino = inode->i_ino;

/* fill the new dir buf*/
    strcpy(buff[0].d_name, ".");
    buff[0].d_ino = inode->i_ino;
    strcpy(buff[1].d_name, "..");
    buff[1].d_ino = curdir.inode->i_ino;

    block = balloc();

    fs.seekp(DATASTART + block * BLOCKSIZ, ios::beg);
    fs.write((char *) buff, sizeof(buff));

    inode->dinode.di_size = 2 * sizeof(direct);
    inode->dinode.di_number = 1;
    inode->dinode.di_mode = DIDIR;
    inode->dinode.di_uid = users[curdir.user_id].u_uid;
    inode->dinode.di_addr[0] = block;

    iput(inode);
}