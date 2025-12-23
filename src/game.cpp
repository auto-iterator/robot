#include "game.h"
#include "console.h"
#include "level.h"
#include "simulator.h"
#include "interface.h"
#include "command.h"
#include "archive.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

static int read_integer(std::string str) {
    std::istringstream iss(str);
    int num; iss >> num;
    if(iss.fail() || !iss.eof())
        throw std::runtime_error("Invalid input.");
    return num;
}

void Game::load_levels() {
    std::ifstream fin("levels.dat");
    int n;
    if(!(fin >> n)) 
        throw std::runtime_error("Failed to read the number of levels.");
    for(int i = 0; i < n; ++i) {
        levels.emplace_back(read_level(fin, i + 1));
    }
    fin.close();

    fin.open("completion.dat");
    if(!fin.good()) 
        throw std::runtime_error("Failed to read completion data.");
    for(int i = 0; i < n; ++i) {
        int x, y, z;
        if(!(fin >> x >> y >> z)) 
            throw std::runtime_error("Failed to read completion data.");
        level_completion.emplace_back(x, y, z);
    }
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

void Game::run() {
    try {
        load_levels();
    }
    catch(std::exception err) {
        std::cout << err.what() << std::endl;
        std::cout << "Game data corrupted." << std::endl;
        // reset_levels();
        std::this_thread::sleep_for(2s);
    }

    draw_start_page();
    Console::wait_key();
    std::string input;
    while(1) {
        draw_level_selection();
        std::getline(std::cin, input);
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
                std::cout << "Invalid level.\n";
                std::this_thread::sleep_for(2s);
                continue;
            }
            enter_level(num - 1);
        }
        catch(std::exception err) {
            std::cout << err.what() << std::endl;
            std::this_thread::sleep_for(2s);
        }
    }
    save_levels();
}