#pragma once
#include"command.h"
#include"level.h"

constexpr int inf = 0x7fffffff;

class Simulator {
    private:
    std::vector<int> in, out, target, area;
    std::vector<Command> command;
    std::map<std::string, int> ava;
    int brick;
    typedef std::vector<Command>::iterator iter;
    iter cur_cmd;
    int robot_pos; // -1 为 inbox，-2 为 outbox
    int cmd_cnt; // 已经执行的指令条数

    void inbox();
    void outbox();
    void add(int);
    void sub(int);
    void copyto(int);
    void copyfrom(int);
    void jump(int);
    int jumpifzero(int);

    public:
    Simulator();
    Simulator(const Simulator&) = default;
    Simulator(const Level&);
};