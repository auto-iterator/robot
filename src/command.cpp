#include "command.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>

std::vector<std::pair<std::string, int>> cmd_arg;

void init_cmd_arg() {
    cmd_arg.emplace_back("inbox", 0);
    cmd_arg.emplace_back("outbox", 0);
    cmd_arg.emplace_back("add", 1);
    cmd_arg.emplace_back("sub", 1);
    cmd_arg.emplace_back("copyto", 1);
    cmd_arg.emplace_back("copyfrom", 1);
    cmd_arg.emplace_back("jump", 1);
    cmd_arg.emplace_back("jumpifzero", 1);
}

void read_cmd(std::istream &in, std::vector<std::string> &command, bool is_keyboard) {
    if(!in.good())
        throw std::runtime_error("Failed to read file.");

    std::string line;
    int label = 1;
    if(is_keyboard) {
        std::cout << "  1 | " << std::flush;
    }
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        if(line == "END") break;
        command.emplace_back(line);
        if(is_keyboard) {
            std::cout << std::setw(3) << ++label << " | " << std::flush;
        }
    }
    std::cout << std::flush;
}

int read_integer(const std::string &str) {
    std::istringstream iss(str);
    int num; iss >> num;
    if(str.size() >= 9 || iss.fail() || !iss.eof())
        throw std::runtime_error("Invalid input.");
    return num;
}


Command resolve_cmd(const std::string &str) {
    std::istringstream ss(str);
    std::string name;
    ss >> name;
    std::vector<int> arg;
    std::string s;
    while(ss >> s) {
        try {
            arg.emplace_back(read_integer(s));
        }
        catch (std::runtime_error err) {
            throw std::runtime_error("Invalid instruction.");
        }
    }
    return Command(name, arg);
}