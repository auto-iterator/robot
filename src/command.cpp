#include "command.h"
#include<sstream>
#include<exception>

void init_cmd_arg() {
    cmd_arg.emplace("inbox", 0);
    cmd_arg.emplace("outbox", 0);
    cmd_arg.emplace("add", 1);
    cmd_arg.emplace("sub", 1);
    cmd_arg.emplace("copyto", 1);
    cmd_arg.emplace("copyfrom", 1);
    cmd_arg.emplace("jump", 1);
    cmd_arg.emplace("jumpifzero", 1);
}

void read_cmd(std::istream &in, std::vector<Command> &command) {
    if(!in.good())
        throw std::runtime_error("Failed to read file.");

    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        if(line == "END") break;
        std::stringstream ss(line);
        std::string name;
        ss >> name;
        std::vector<int> arg;
        int x;
        while(ss >> x) arg.emplace_back(x);
        command.emplace_back(name, arg);
    }
}