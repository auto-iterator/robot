#ifndef COMMAND_H
#define COMMAND_H

#include<string>
#include<vector>
#include<map>

struct Command {
    std::string name;
    std::vector<int> arg;
    Command() = default;
    Command(const Command&) = default;
    Command(std::string &s, std::vector<int> &a) :
        name(s), arg(a) {}
};

std::map<std::string, int> cmd_arg;

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

#endif