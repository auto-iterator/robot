#include"customize.h"

template<typename T>
int read(std::ifstream &fin, T &x) {
    if(fin >> x) return 0;
    return 1;
}

int read_level(std::ifstream &fin, Level &level) {
    int n;

    if(read(fin, n)) return 1;
    std::vector<int> in;
    for(int i = 1, x; i <= n; ++i) {
        if(read(fin, x)) return 1;
        in.emplace_back(x);
    }

    if(read(fin, n)) return 1;
    std::vector<int> target;
    for(int i = 1, x; i <= n; ++i) {
        if(read(fin, x)) return 1;
        target.emplace_back(x);
    }

    int area_num;
    if(read(fin, area_num)) return 1;

    if(read(fin, n)) return 1;
    std::map<std::string, int> ava_cmd;
    std::string s;
    for(int i = 1; i <= n; ++i) {
        if(read(fin, s)) return 1;
        auto it = cmd_arg.find(s);
        if(it == cmd_arg.end()) return 1;
        ava_cmd.emplace(it);
    }

    getline(fin, s);
    getline(fin, s); // description

    level = Level(in, target, area_num, ava_cmd, s);
    return 0;
}

void write_level(std::ofstream &fout, Level &level) {
    using std::endl;
    fout << level.in.size() << endl;
    for(auto i : level.in) fout << i << ' ';
    fout << endl;

    fout << level.target.size() << endl;
    for(auto i : level.target) fout << i << ' ';
    fout << endl;

    fout << level.ava_cmd.size() << endl;
    for(auto i : level.ava_cmd) fout << i.first << ' ';
    fout << endl;

    fout << level.area_num << endl;

    fout << level.description << endl;
}