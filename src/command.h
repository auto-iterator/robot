#pragma once

#include<string>
#include<vector>

struct Command {
    std::string name;
    std::vector<int> arg;

    Command() = default;

    Command(const Command&) = default;

    Command(std::string &s, std::vector<int> &a) :
        name(s), arg(a) {}
};

extern std::vector<std::pair<std::string, int>> cmd_arg;

void init_cmd_arg();

void read_cmd(std::istream&, std::vector<std::string>&, bool);

int read_integer(const std::string&);

Command resolve_cmd(const std::string&);