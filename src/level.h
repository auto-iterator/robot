#pragma once

#include<vector>
#include<string>
#include<map>

struct Level {
    int id;
    std::vector<int> in, target;
    std::map<std::string, int> ava_cmd;
    int area_num;
    std::string description;

    Level(): id(0), area_num(0) {}

    Level(const Level&) = default;
    
    Level(const int ID, const std::vector<int> &IN, const std::vector<int> &TAR, const int &AREA_N,
        std::map<std::string, int> &AVA_CMD, const std::string &DES, const int &p = 0) :
        id(ID), in(IN), target(TAR), ava_cmd(AVA_CMD), area_num(AREA_N), description(DES) {}
        
    Level& operator=(const Level&) = default;
};

struct Pass_info {
    int passed;
    int min_time;
    int min_code;

    Pass_info(): passed(0), min_time(0), min_code(0) {}

    Pass_info(const Pass_info&) = default;

    Pass_info(const int &PASSED, const int &MIN_TIME, const int &MIN_CODE) :
        passed(PASSED), min_time(MIN_TIME), min_code(MIN_CODE) {}

    Pass_info& operator=(const Pass_info&) = default;
};