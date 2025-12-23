#include "simulator.h"
#include "command.h"
#include <fstream>
#include <stdexcept>

Simulator::Simulator(const Level& level, std::istream &fin) {
    in = level.in;
    target = level.target;
    area.resize(level.area_num);
    for(auto &i: area) i = inf;
    ava = level.ava_cmd;
    brick = inf;
    read_cmd(fin, command);
    cur = command.begin();
    robot_pos = -1;
    cmd_cnt = 0;
}

bool Simulator::inbox() {
    if(in.empty()) return 1;
    brick = *in.begin();
    in.erase(in.begin());
    ++cur;
    robot_pos = -1;
    return 0;
}

void Simulator::outbox() {
    if(brick == inf) 
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    out.push_back(brick);
    brick = inf;
    ++cur;
    robot_pos = -2;
}

void Simulator::add(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(brick == inf)
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    brick += area[x];
    ++cur;
    robot_pos = x;
}

void Simulator::sub(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(brick == inf)
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    brick -= area[x];
    ++cur;
    robot_pos = x;
}

void Simulator::copyto(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(brick == inf)
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    area[x] = brick;
    ++cur;
    robot_pos = x;
}

void Simulator::copyfrom(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    brick = area[x];
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
    if(brick == inf)
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    if(brick != 0) ++cur;
    else cur = command.begin() + x - 1;
}

bool Simulator::step() {
    if(cur == command.end()) return 0;
    if(ava.find(cur->name) == ava.end())
        throw Simulator_Exception("No such command or command not available.", cur - command.begin() + 1);
    if(ava[cur->name] != cur->arg.size())
        throw Simulator_Exception("Wrong number of parameters.", cur - command.begin() + 1);

    if(cur->name == "inbox") {
        if(inbox()) return 0;
    }
    else if(cur->name == "outbox") outbox();
    else if(cur->name == "add") add(cur->arg[0]);
    else if(cur->name == "sub") sub(cur->arg[0]);
    else if(cur->name == "copyto") copyto(cur->arg[0]);
    else if(cur->name == "copyfrom") copyfrom(cur->arg[0]);
    else if(cur->name == "jump") jump(cur->arg[0]);
    else if(cur->name == "jumpifzero") jumpifzero(cur->arg[0]);

    ++cmd_cnt;

    if(brick > inf)
        throw Simulator_Exception("Brick too big.", cur - command.begin() + 1);
    if(brick < -inf)
        throw Simulator_Exception("Brick too small.", cur - command.begin() + 1);

    return 1;
}

std::string Simulator::success() {
    if(!in.empty()) return "Fail: inbox not empty";
    if(out != target) return "Fail: target not achieved.";
    return "Success!";
}