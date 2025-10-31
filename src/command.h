#pragma once

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

void init_cmd_arg();

void read_cmd(std::istream&, std::vector<Command>&);