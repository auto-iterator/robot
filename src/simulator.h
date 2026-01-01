#pragma once
#include "command.h"
#include "level.h"
#include <exception>
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
    std::vector<std::string> command;
    std::vector<std::pair<std::string, int>> ava;
    int block;
    typedef std::vector<std::string>::iterator iter;
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
    Simulator(const Level&, std::vector<std::string>&);
    bool step();
    std::string success() const;
    int get_cnt() const;
    int get_cur() const;
    int get_robot_pos() const;
    int get_block() const;
    const std::vector<std::string>& get_cmd() const;
    const std::vector<int>& get_inbox() const;
    const std::vector<int>& get_outbox() const;
    const std::vector<int>& get_target() const;
    const std::vector<int>& get_area() const;
};