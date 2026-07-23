#include "console.h"
#include <cstdio>
#include <string>
#include <utility>
#include <iostream>

#if defined(_WIN32) or defined(_WIN64)
    #include <conio.h>
    int Console::wait_key() {
        return _getch();
    }
#else
    #include <termios.h>
    #include <unistd.h>
    int Console::wait_key() {
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

void Console::move_to(int x, int y) {
    // _x = x, _y = y;
    printf(CSI"%d;%dH", x, y);
}

void Console::erase_cur() {
    printf(" \b");
}

void Console::erase_point(int x, int y) {
    move_to(x, y);
    erase_cur();
}

void Console::erase_rect(int x, int y, int width, int height) {
    char *space = new char[width + 1];
    for(int i = 0; i <= width; ++i)
        space[i] = " "[i == width];

    for(int i = 0; i < height; ++i) {
        move_to(x + i, y);
        printf("%s", space);
    }
    delete[] space;
    // _x = x + height - 1;
    // _y = y + width - 1;
}

void Console::clear_screen() {
    printf(CSI"2J");
}

// width 为上下边框中 '-' 的个数，height 为左右边框中 '|' 的个数
void Console::draw_box(int x, int y, int width, int height) {
    erase_rect(x, y, width + 2, height + 2);

    move_to(x, y);
    putchar('+');
    for(int i = 1; i <= width; ++i) 
        putchar('-');
    putchar('+');

    for(int i = 1; i <= height; ++i) {
        move_to(x + i, y);
        putchar('|');
        move_to(x + i, y + width + 1);
        putchar('|');
    }

    move_to(x + height + 1, y);
    putchar('+');
    for(int i = 1; i <= width; ++i)
        putchar('-');
    putchar('+');
}

void Console::draw_line(int x, int y, int len, char ch) {
    move_to(x, y);
    for(int i = 0; i < len; ++i)
        putchar(ch);
}

// 如果 val 过长 (>len) 就左对齐，否则居中
void Console::write_str(int x, int y, int len, const std::string &str) {
    erase_rect(x, y, len, 1);
    if(str.size() > len) {
        move_to(x, y);
        for(int i = 0; i < len; ++i)
            putchar(str[i]);
    }
    else {
        int res = (len - str.size()) / 2;
        move_to(x, y + res);
        std::cout << str;
    }
}

void Console::draw_fig(int x, int y, const std::vector<std::string> &fig) {
    for(auto str : fig) {
        move_to(x, y);
        std::cout << str;
        ++x;
    }
}

#ifdef DEBUG

void Console::test() {
    clear_screen();
    move_to(0,0);
    std::cout << "IEE";
    move_to(3,4);
    std::cout << "EEI";
    move_to(3,5);
    erase_cur();
    for(int i = 1; i <= 100; ++i)
        std::cout << "HATSUNEMIKU";
    erase_point(5, 5);
    draw_line(6, 6, 10, '=');
    draw_box(10, 10, 5, 3);
    write_str(11, 11, 5, "123");
    
}

#endif