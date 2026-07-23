// level.h

#include<vector>
#include<string>
#include<map>

struct Level {
    int id;
    std::vector<int> in, target;
    std::map<std::string, int> ava_cmd;
    int area_num;
    std::string description;
    int passed;

    Level() {
        id = area_num = passed = 0;
    }

    Level(const Level&) = default;
    
    Level(const std::vector<int> &i, const std::vector<int> &t, const int &n,
        std::map<std::string, int> &a, const std::string &d, const int &p) :
        id(0), in(i), target(t), ava_cmd(a), area_num(n), description(d), passed(p) {}
        
    Level& operator=(const Level&) = default;
};

// command.h

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

std::map<std::string, unsigned> cmd_arg;

void init_cmd_arg();

void read_cmd(std::istream&, std::vector<Command>&);

// command.cpp

#include <sstream>
#include <exception>

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

// simulator.h

#include <stdexcept>

constexpr int inf = 0x3f3f3f3f; // 某个值 == inf 意味着该值不存在

class Simulator_Exception {
    public:
    std::runtime_error err;
    int pos;
    Simulator_Exception(const std::string &s, const int &x):
        err(s), pos(x) {}
};

class Simulator {
    private:
    std::vector<int> in, out, target, area;
    std::vector<Command> command;
    std::map<std::string, int> ava;
    int brick;
    typedef std::vector<Command>::iterator iter;
    iter cur;
    int robot_pos; // -1 为 inbox，-2 为 outbox
    int cmd_cnt; // 已经执行的指令条数

    bool inbox();
    void outbox();
    void add(int);
    void sub(int);
    void copyto(int);
    void copyfrom(int);
    void jump(int);
    void jumpifzero(int); 

    public:
    Simulator();
    Simulator(const Simulator&) = default;
    Simulator(const Level&, std::istream&);
    bool step();
    std::string success();
};

// simulator.cpp

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
    return 0;
}

void Simulator::outbox() {
    if(brick == inf) 
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    out.push_back(brick);
    brick = inf;
    ++cur;
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
}

void Simulator::copyto(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(brick == inf)
        throw Simulator_Exception("Brick not held.", cur - command.begin() + 1);
    area[x] = brick;
    ++cur;
}

void Simulator::copyfrom(int x) {
    if(x < 0 || area.size() <= x)
        throw Simulator_Exception("No such area.", cur - command.begin() + 1);
    if(area[x] == inf) 
        throw Simulator_Exception("Area empty.", cur - command.begin() + 1);
    brick = area[x];
    ++cur;
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

// OJ test

#include <iostream>

int main() {
    int T; 
    std::cin >> T;

    Level level;

    if(T == 1) {
        level.in = {1, 2};
        level.target = {1, 2};
        level.area_num = 0;
        level.ava_cmd.emplace("inbox", 0);
        level.ava_cmd.emplace("outbox", 0);
    }
    else if(T == 2) {
        level.in = {3, 9, 5, 1, -2, -2, 9, -9};
        level.target = {-6, 6, 4, -4, 0, 0, 18, -18};
        level.area_num = 3;
        level.ava_cmd.emplace("inbox", 0);
        level.ava_cmd.emplace("outbox", 0);
        level.ava_cmd.emplace("add", 1);
        level.ava_cmd.emplace("sub", 1);
        level.ava_cmd.emplace("copyto", 1);
        level.ava_cmd.emplace("copyfrom", 1);
        level.ava_cmd.emplace("jump", 1);
        level.ava_cmd.emplace("jumpifzero", 1);
    }
    else if(T == 3) {
        level.in = {6, 2, 7, 7, -9, 3, -3, -3};
        level.target = {7, -3};
        level.area_num = 3;
        level.ava_cmd.emplace("inbox", 0);
        level.ava_cmd.emplace("outbox", 0);
        level.ava_cmd.emplace("add", 1);
        level.ava_cmd.emplace("sub", 1);
        level.ava_cmd.emplace("copyto", 1);
        level.ava_cmd.emplace("copyfrom", 1);
        level.ava_cmd.emplace("jump", 1);
        level.ava_cmd.emplace("jumpifzero", 1);
    }
 
    int m;
    std::cin >> m;
    std::string line;
    std::getline(std::cin, line);
    
    Simulator simulator(level, std::cin);
    
    try {
        while(simulator.step());
    }
    catch (Simulator_Exception err) {
        std::cout << "Error on instruction " << err.pos << std::endl;
        return 0;
    }
    if(simulator.success() == "Success!") std::cout << "Success";
    else std::cout << "Fail";
}