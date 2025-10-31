#ifndef LEVEL_H
#define LEVEL_H

#include<vector>
#include<string>
#include<map>

struct Level {
    int id;
    std::vector<int> in, target;
    std::map<std::string, int> ava_cmd;
    int area_num;
    std::string description;

    Level() {
        id = area_num = 0;
    }
    Level(const Level&) = default;
    Level(const std::vector<int> &i, const std::vector<int> &t, const int &n,
        std::map<std::string, int> &a, const std::string &d) :
        id(0), in(i), target(t), ava_cmd(a), area_num(n), description(d) {}
    Level& operator=(const Level&) = default;
};

#endif