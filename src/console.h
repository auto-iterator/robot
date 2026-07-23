#pragma once

#include <cstdio>
#include <vector>
#include <string>

#define CSI "\033["

namespace Console {

    int wait_key();
    void move_to(int, int);
    void erase_cur();
    void erase_point(int, int);
    void erase_rect(int, int, int, int);
    void clear_screen();
    void draw_box(int, int, int, int);
    void draw_line(int, int, int, char);
    void write_str(int, int, int, const std::string&);
    void draw_fig(int, int, const std::vector<std::string>&);

#ifdef DEBUG
    void test();
#endif

};