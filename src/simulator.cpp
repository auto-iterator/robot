#include "simulator.h"
#include "command.h"
#include "console.h"
#include <exception>
#include <fstream>
#include <stdexcept>
#include <vector>

Simulator::Simulator(const Level& level, std::vector<std::string> &cmd) {
    in = level.in;
    target = level.target;
    area.resize(level.area_num);
    for(auto &i: area) i = inf;
    ava = level.ava_cmd;
    block = inf;
    command = cmd;
    cur = command.begin();
    robot_pos = -1;
    cmd_cnt = 0;
}

bool Simulator::inbox() {
    if(in.empty()) return 1;
    block = *in.begin();
    in.erase(in.begin());
    ++cur;
    robot_pos = -1;
    return 0;
}

void Simulator::outbox() {
    if(block == inf) 
        throw Simulator_Exception("No block held.", cur - command.begin() + 1);
    out.push_back(block);
    block = inf;
    ++cur;
    robot_pos = -2;
}

void Simulator::add(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(block == inf)
        throw Simulator_Exception("No block held.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    block += area[x];
    ++cur;
    robot_pos = x;
}

void Simulator::sub(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(block == inf)
        throw Simulator_Exception("No block held.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    block -= area[x];
    ++cur;
    robot_pos = x;
}

void Simulator::copyto(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(block == inf)
        throw Simulator_Exception("No block held.", cur - command.begin() + 1);
    area[x] = block;
    ++cur;
    robot_pos = x;
}

void Simulator::copyfrom(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    block = area[x];
    ++cur;
    robot_pos = x;
}

void Simulator::jump(int x) {
    if(x <= 0 || command.size() < x) 
        throw Simulator_Exception("No such line number.", cur - command.begin() + 1);
    cur = command.begin() + x - 1;
}

void Simulator::jumpifzero(int x) {
    if(x <= 0 || command.size() < x)
        throw Simulator_Exception("No such line number.", cur - command.begin() + 1);
    if(block == inf)
        throw Simulator_Exception("No block held.", cur - command.begin() + 1);
    if(block != 0) ++cur;
    else cur = command.begin() + x - 1;
}

bool Simulator::step() {
    if(cur == command.end()) return 0;
    Command cmd;
    try {
        cmd = resolve_cmd(*cur);
    }
    catch (std::runtime_error err) {
        throw Simulator_Exception(err.what(), cur - command.begin() + 1);
    }

    auto it = ava.begin();
    for(; it != ava.end(); ++it)
        if(it->first == cmd.name) 
            break;
    if(it == ava.end())
        throw Simulator_Exception("No such instruction or instruction not available.", cur - command.begin() + 1);
    if(it->second != cmd.arg.size())
        throw Simulator_Exception("Wrong number of parameters.", cur - command.begin() + 1);

    if(cmd.name == "inbox") {
        if(inbox()) return 0;
    }
    else if(cmd.name == "outbox") outbox();
    else if(cmd.name == "add") add(cmd.arg[0]);
    else if(cmd.name == "sub") sub(cmd.arg[0]);
    else if(cmd.name == "copyto") copyto(cmd.arg[0]);
    else if(cmd.name == "copyfrom") copyfrom(cmd.arg[0]);
    else if(cmd.name == "jump") jump(cmd.arg[0]);
    else if(cmd.name == "jumpifzero") jumpifzero(cmd.arg[0]);

    ++cmd_cnt;

    if(block > inf)
        throw Simulator_Exception("Block too big.", cur - command.begin() + 1);
    if(block < -inf)
        throw Simulator_Exception("Block too small.", cur - command.begin() + 1);

    return 1;
}

std::string Simulator::success() const {
    if(!in.empty()) return "Fail: inbox not empty";
    if(out != target) return "Fail: target not achieved.";
    return "Success!";
}

int Simulator::get_cnt() const {
    return cmd_cnt;
}

int Simulator::get_cur() const {
    return cur - command.begin() + 1;
}

int Simulator::get_robot_pos() const {
    return robot_pos;
}

int Simulator::get_block() const {
    return block;
}

const std::vector<std::string>& Simulator::get_cmd() const {
    return command;
}

const std::vector<int>& Simulator::get_inbox() const {
    return in;
}

const std::vector<int>& Simulator::get_outbox() const {
    return out;
}

const std::vector<int>& Simulator::get_area() const {
    return area;
}

const std::vector<int>& Simulator::get_target() const {
    return target;
}