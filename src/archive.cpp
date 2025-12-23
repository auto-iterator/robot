#include "archive.h"
#include "level.h"
#include <exception>
#include <stdexcept>

Level read_level(std::ifstream &fin, int id) {
    if(!fin.good())
        throw std::runtime_error("Failed to read file.");
    int n;

    if(!(fin >> n)) 
        throw std::runtime_error("Failed to read the length of inbox.");
    std::vector<int> in;
    for(int i = 1, x; i <= n; ++i) {
        if(!(fin >> x)) 
            throw std::runtime_error("Failed to read inbox element.");
        in.emplace_back(x);
    }

    if(!(fin >> n)) 
        throw std::runtime_error("Failed to read the length of target.");
    std::vector<int> target;
    for(int i = 1, x; i <= n; ++i) {
        if(!(fin >> x)) 
            throw std::runtime_error("Failed to read target element.");
        target.emplace_back(x);
    }

    int area_num;
    if(!(fin >> area_num)) 
        throw std::runtime_error("Failed to read the number of blank areas.");

    if(!(fin >> n)) 
        throw std::runtime_error("Failed to read the number of available commands.");
    std::map<std::string, int> ava_cmd;
    std::string s;
    for(int i = 1; i <= n; ++i) {
        if(!(fin >> s)) 
            throw std::runtime_error("Failed to read command name.");
        auto it = cmd_arg.find(s);
        if(it == cmd_arg.end())
            throw std::runtime_error("Invalid command name.");
        ava_cmd.emplace(*it);
    }

    getline(fin, s);
    getline(fin, s); // description

    return Level(id, in, target, area_num, ava_cmd, s);
}

void write_level(std::ofstream &fout, Level &level) {
    if(!fout.good())
        throw std::runtime_error("Failed to write file.");

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