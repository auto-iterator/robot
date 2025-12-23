#pragma once

#include "level.h"

class Game {
    std::vector<Level> levels;
    std::vector<Pass_info> level_completion;

    void load_levels();
    void save_levels();
    // void reset_levels();

    void enter_level(int);
    void create_level();
    void delete_level();

    public:

    Game();
    void init();
    void run();
};