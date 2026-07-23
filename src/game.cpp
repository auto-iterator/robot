#include "game.h"
#include "console.h"
#include "level.h"
#include "simulator.h"
#include "interface.h"
#include "command.h"
#include "archive.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std::chrono_literals;

void Game::load_levels() {
    std::ifstream fin("levels.dat");
    if(!fin.good()) 
        throw std::runtime_error("Failed to open levels.dat");
    int n;
    if(!(fin >> n)) 
        throw std::runtime_error("Failed to read the number of levels.");
    for(int i = 0; i < n; ++i) {
        levels.emplace_back(read_level(fin, i + 1));
    }
    fin.close();

    fin.open("completion.dat");
    if(!fin.good()) 
        throw std::runtime_error("Failed to open completion.dat");
    for(int i = 0; i < n; ++i) {
        int x, y, z;
        if(!(fin >> x >> y >> z)) 
            throw std::runtime_error("Failed to read completion data.");
        level_completion.emplace_back(x, y, z);
    }
    
    if(levels.size() != level_completion.size()) 
        throw std::runtime_error("levels.dat and completion.dat do not match.");
}

void Game::save_levels() {
    std::ofstream fout("levels.dat");
    if(!fout.good())
        throw std::runtime_error("Failed to write levels.dat");
    int n = levels.size();
    fout << n << std::endl;
    for(int i = 0; i < n; ++i) {
        write_level(fout, levels[i]);
    }
    fout.close();

    fout.open("completion.dat");
    if(!fout.good()) 
        throw std::runtime_error("Failed to write completion.dat");
    for(auto [x, y, z] : level_completion) {
        fout << x << ' ' << y << ' ' << z << std::endl;
    }
}

void Game::reset_levels() {
    levels.clear();
    level_completion.clear();

    std::vector<int> in, tar;
    std::vector<std::pair<std::string, int>> ava;
    // 1
    in = tar = {1, 2};
    ava.emplace_back("inbox", 0);
    ava.emplace_back("outbox", 0);
    levels.emplace_back(1, in, tar, 0, ava, 
        "Take out each brick in inbox and put it into outbox.");
    // 2
    in = {3, 9, 5, 1, -2, -2, 9, -9};
    tar = {-6, 6, 4, -4, 0, 0, 18, -18};
    levels.emplace_back(2, in, tar, 3, cmd_arg, 
        "For every two things in inbox, subtract the first from the second and put the result into outbox, then subtract the second from the first and put the result into outbox. Repeat.");
    // 3
    in = {6, 2, 7, 7, -9, 3, -3, -3};
    tar = {7, -3};
    levels.emplace_back(3, in, tar, 3, cmd_arg, 
        "Take two numbers from inbox in turn. If they are equal, output one of them, otherwise discard them. Repeat.");
    // 4
    // TODO
    in = {3, 5, -2, -8, 9, 0, -8, -10};
    tar = {-3, -5, 2, 8, -9, 0, 8, 10};
    levels.emplace_back(4, in, tar, 1, cmd_arg, 
        "Output the opposite number of each number in inbox.");

    level_completion = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    // std::cerr << levels.size() << std::endl;
}

void Game::create_level() {
    while(1) {
        draw_import_from_file();
        std::string input;
        std::getline(std::cin, input);
        if(input == "b" || input == "B") {
            return;
        }
        // else if(input == "2") {
            // draw_import_from_file();
            // std::getline(std::cin, input);
            std::ifstream fin(input);
            try {
                levels.emplace_back(read_level(fin, levels.size()));
                level_completion.emplace_back(0, 0, 0);
                fin.close();
                save_levels();
                return;
            }
            catch(std::runtime_error err) {
                std::cout << err.what() << std::endl;
                std::this_thread::sleep_for(1.5s);
                fin.close();
            }
        // }
        // else if(input == "1") {
        //     draw_level_creation();
        //     // TODO
        // }
    }
}

void Game::delete_level() {
    while(1) {
        draw_level_deletion(levels);
        std::string input;
        std::getline(std::cin, input);
        try {
            if(input == "b" || input == "B") {
                return;
            }
            if(input == "a" || input == "A") {
                std::cout << "Sure to delete all custom levels? (y/n)" << std::endl;
                std::getline(std::cin, input);
                if(input != "y" && input != "Y") {
                    std::cout << "Deletion cancelled." << std::endl;
                    std::this_thread::sleep_for(1.5s);
                    return;
                }
                while(levels.size() > 4)
                    levels.pop_back(), level_completion.pop_back();
                save_levels();
                return;
            }
            int res = read_integer(input);
            if(res <= 4 || res > levels.size()) {
                std::cout << "Invalid level." << std::endl;;
                std::this_thread::sleep_for(1.5s);
            }
            else {
                std::cout << "Sure to delete level " << res << "? (y/n)" << std::endl;
                std::getline(std::cin, input);
                if(input != "y" && input != "Y") {
                    std::cout << "Deletion cancelled." << std::endl;
                    std::this_thread::sleep_for(1.5s);
                    return;
                }
                levels.erase(levels.begin() + res - 1);
                level_completion.erase(level_completion.begin() + res - 1);
                int i = 1;
                for(auto &lvl : levels) {
                    lvl.id = i++; // 重新编号
                }
                save_levels();
                return;
            }
        }
        catch(std::runtime_error err) {
            std::cout << err.what() << std::endl;
            std::this_thread::sleep_for(1.5s);
        }
    }
}

void Game::enter_level(int num) {
    draw_enter_level(levels[num]);
    std::string input;
    std::getline(std::cin, input);
    
    std::vector<std::string> cmd;
    if(input == "b" || input == "B") {
        return;
    }
    if(input == "1") {
        std::cout << "Input file directory: ";
        std::getline(std::cin, input);
        std::ifstream fin(input);
        if(!fin.good()) {
            std::cout << "Failed to read file." << std::endl;
            std::this_thread::sleep_for(1.5s);
            return;
        }
        read_cmd(fin, cmd, 0);
        fin.close();
    }
    else if(input == "2") {
        draw_enter_code(levels[num]);
        read_cmd(std::cin, cmd, 1);
        // std::cerr << "INPUT END" << std::endl;
    }
    else return;

    Simulator simulator(levels[num], cmd);

    try {
        int l = 1, r = 18;
        draw_game_frame(simulator, levels[num], l, r);
        std::this_thread::sleep_for(1s);
        while(simulator.step()) {
            draw_game_frame(simulator, levels[num], l, r);
            std::this_thread::sleep_for(1s);
        }
    }
    catch (Simulator_Exception err) {
        std::cout << "Error on instruction " << err.pos << ": " << err.err.what();
        std::cout << std::endl << "Press any key to return to the menu..." << std::flush;
        Console::wait_key();
        return;
    }

    std::cout << "Simulation done." << std::endl;
    std::cout << "Press any key to continue..." << std::flush;
    Console::wait_key();

    auto res = simulator.success();
    if(res == "Success!") {
        auto &info = level_completion[num];
        draw_level_success(info, {1, simulator.get_cnt(), cmd.size()}, num + 1);
        if(info.passed) {
            info.min_time = std::min(info.min_time, simulator.get_cnt());
            info.min_code = std::min(info.min_code, (int)cmd.size());
        }
        else {
            info = {1, simulator.get_cnt(), (int)cmd.size()};
        }
        save_levels();
        Console::wait_key();
    }
    else {
        draw_level_fail(res, simulator, num + 1);
        Console::wait_key();
    }
}

void Game::run() {
    init_cmd_arg();

    try {
        load_levels();
    }
    catch(std::runtime_error err) {
        std::cout << err.what() << std::endl;
        std::cout << "Resetting level data..." << std::endl;
        reset_levels();
        save_levels();
        std::this_thread::sleep_for(1.5s);
    }

    draw_start_page();
    Console::wait_key();
    std::string input;
    while(1) {
        draw_level_selection(levels, level_completion);
        std::getline(std::cin, input);
        if(input == "q" || input == "Q") {
            break;
        }
        if(input == "h" || input == "H") {
            view_help();
            Console::wait_key();
            continue;
        }
        if(input == "+") {
            create_level();
            continue;
        }
        if(input == "-") {
            delete_level();
            continue;
        }
        std::istringstream iss(input);
        try {
            int num = read_integer(input);
            if(num <= 0 || num > levels.size()) {
                std::cout << "Invalid level." << std::endl;
                std::this_thread::sleep_for(1.5s);
                continue;
            }
            if(num <= 4 && num > 1 && !level_completion[num - 2].passed) {
                std::cout << "Level locked." << std::endl;
                std::this_thread::sleep_for(1.5s);
            }
            else {
                enter_level(num - 1);
            }
        }
        catch(std::runtime_error err) {
            std::cout << err.what() << std::endl;
            std::this_thread::sleep_for(1.5s);
        }
    }
    save_levels();
}