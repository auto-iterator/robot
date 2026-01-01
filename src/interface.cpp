#include "interface.h"
#include "console.h"
#include "level.h"
#include "simulator.h"
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <sstream>
#include <string>
#include <iomanip>

static const int CONSOLE_WIDTH = 80;
static const int CONSOLE_HEIGHT = 26;

std::vector<std::string> split_string_to_fit(const std::string& text, int width) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string word;
    std::string line;
    
    while (stream >> word) {
        if (line.length() + word.length() + 1 > width) {
            lines.push_back(line);
            line = word;
        } else {
            if (!line.empty()) line += " ";
            line += word;
        }
    }
    if (!line.empty()) lines.push_back(line);
    
    return lines;
}

void draw_start_page() {
    Console::clear_screen();
    
    // 绘制顶部边框
    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    
/*
 _____         _             _____   _               _
|  __ \       | |           |  __ \ | |             | |
| |__) | ___  | |__    ___  | |__) || |  ___    ____| |__  ____
|  _  / / _ \ |  _ \  / _ \ |  __ | | | / _ \  / __/| / / / __/
| | \ \| |_| || |_| || (_) || |__) \| || (_) || (__ | \ \ \__ \
|_|  \_\\___/ |____/  \___/ |______/|_| \___/  \___\|_|\_\\___/
*/
    static const std::vector<std::string> title = {
        R"RAW(  _____         _             _____   _               _          )RAW",
        R"RAW( |  __ \       | |           |  __ \ | |             | |         )RAW",
        R"RAW( | |__) | ___  | |__    ___  | |__) /| |  ___    ____| |__  ____ )RAW",
        R"RAW( |  _  / / _ \ |  _ \  / _ \ |  __ | | | / _ \  / __/| / / / __/ )RAW",
        R"RAW( | | \ \| (_) || |_) || (_) || |__) \| || (_) || (__ | \ \ \__ \ )RAW",
        R"RAW( |_|  \_\\___/ |____/  \___/ |______/|_| \___/  \___\|_|\_\\___/ )RAW",
        R"RAW(                                                                 )RAW",
        R"RAW(                 A ROBOT PROGRAMMING SIMULATOR                   )RAW"
    };
    
    Console::draw_fig(7, (CONSOLE_WIDTH - title[0].size()) / 2, title);
    
    Console::move_to(19, 24);
    std::cout << "Press any key to continue...";
    
    // 绘制底部边框
    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    std::cout << std::flush;
}

void draw_level_selection(const std::vector<Level> &levels,
const std::vector<Pass_info> &level_completion) {
    Console::clear_screen();
    
    // 绘制标题
    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 15) / 2);
    std::cout << "LEVEL SELECTION";
    
    // 绘制预设关卡
    Console::move_to(3, 10);
    std::cout << "PRESET LEVELS:";
    
    for (int i = 0; i < 4; ++i) { 
        Console::move_to(5 + i, 10);
        std::cout << "Level " << i + 1;
        
        Console::move_to(5 + i, 20);
        if(level_completion[i].passed) {
            std::cout << "[PASSED]";
            std::cout << " Minimium Code: " << level_completion[i].min_code;
            std::cout << " Shortest Time: " << level_completion[i].min_time;
        }
        else if(i > 0 && !level_completion[i - 1].passed) {
            std::cout << "[LOCKED]";
        }
        else {
            std::cout << "[AVAILABLE]";
        }
    }
    
    // 绘制自定义关卡
    Console::move_to(10, 10);
    std::cout << "CUSTOM LEVELS:";

    if(levels.size() == 4) {
        Console::move_to(12, 10);
        std::cout << "Empty";
    }
    else {
        for(int i = 4; i < levels.size() && i < 8; ++i) {
            Console::move_to(8 + i, 10);
            std::cout << "Level " << i + 1;

            Console::move_to(8 + i, 20);
            if(level_completion[i].passed) {
                std::cout << "[PASSED]";
                std::cout << " Minimium Code: " << level_completion[i].min_code;
                std::cout << " Shortest Time: " << level_completion[i].min_time;
            }
            else {
                std::cout << "[AVAILABLE]";
            }
        }
        if(levels.size() > 8) {
            Console::move_to(16, 10);
            std::cout << "(" << levels.size() - 8 << " Folded)";
        }
    }
    
    // 绘制说明
    Console::draw_box(17, 10, 60, 6);

    std::vector<std::string> text = {
        "Instructions:",
        "Enter level number to select",
        "Enter '+' to create a level",
        "Enter '-' to delete a level",
        "Enter 'h' to view help",
        "Enter 'q' to quit the game"
    };

    Console::draw_fig(18, 12, text);
    
    // 绘制底部边框
    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    
    // 提示输入
    Console::move_to(26, 0);
    std::cout << "Input: ";

    std::cout << std::flush;
}

// void draw_select_level_creation_method() {
//     Console::clear_screen();
    
//     // 绘制标题
//     Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
//     Console::move_to(1, (CONSOLE_WIDTH - 25) / 2);
//     std::cout << "LEVEL CREATION METHOD";
    
//     // 绘制选项框
//     Console::draw_box(5, 19, 42, 11);
    
//     std::vector<std::string> options = {
//         "Choose creation method:",
//         "",
//         "1. Manual Input",
//         "   - Define all parameters manually",
//         "",
//         "2. Import from File",
//         "   - Load level configuration from file",
//         "",
//         "b. Back to Main Menu"
//     };
    
//     Console::draw_fig(7, 22, options);
    
//     // 绘制底部边框
//     Console::draw_line(20, 0, CONSOLE_WIDTH, '=');
    
//     // 提示输入
//     Console::move_to(21, 0);
//     std::cout << "Input: ";
// }

void draw_import_from_file() {
    Console::clear_screen();

    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 12) / 2);
    std::cout << "CREATE LEVEL";
    
    Console::move_to(3, 18);
    std::cout << "Import level data from file.";
    Console::move_to(4, 18);
    std::cout << "The input file must follow the following format:";

    Console::draw_box(6, 18, 45, 10);

    std::vector<std::string> format = {
        "Inbox size (a number)",
        "Inbox elements (separated by space)",
        "Outbox size (a number)",
        "Outbox elements (separated by space)",
        "Open area number (no more than 4)",
        "Available instruction number (a number)",
        "Available instructions (separated by space)",
        "Level description (in one line)"
    };

    Console::draw_fig(8, 20, format);

    Console::move_to(19, 18);
    std::cout << "Enter 'b' to go back";

    Console::draw_line(21, 0, CONSOLE_WIDTH, '=');

    Console::move_to(22, 0);
    std::cout << "Input file directory: ";

    std::cout << std::flush;
}

// void draw_level_creation() {
//     Console::clear_screen();
    
//     // 绘制标题
//     Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
//     Console::move_to(1, (CONSOLE_WIDTH - 20) / 2);
//     std::cout << "CREATE NEW LEVEL";
    
//     // 绘制输入表单
//     Console::draw_box(3, 5, 70, 18);
    
//     // 表单标题
//     Console::move_to(4, 7);
//     std::cout << "Level Configuration:";
    
//     // 各个配置项
//     std::vector<std::string> config_items = {
//         "Level Name: ",
//         "Input Sequence (comma-separated): ",
//         "Target Output Sequence: ",
//         "Number of Storage Areas: ",
//         "Available Commands (comma-separated): ",
//         "Description: "
//     };
    
//     for (int i = 0; i < config_items.size(); ++i) {
//         Console::move_to(6 + i * 2, 7);
//         std::cout << config_items[i];
        
//         // 绘制输入区域下划线
//         Console::draw_line(7 + i * 2, 7 + config_items[i].length(), 
//                           50 - config_items[i].length(), '_');
//     }
    
//     // 绘制可用指令提示
//     Console::move_to(18, 7);
//     std::cout << "Available commands: inbox, outbox, add, sub,";
//     Console::move_to(19, 7);
//     std::cout << "copyto, copyfrom, jump, jumpifzero";
    
//     // 绘制底部操作提示
//     Console::draw_line(20, 0, CONSOLE_WIDTH, '-');
//     Console::move_to(21, 10);
//     std::cout << "Press 's' to save, 'c' to cancel: ";
    
//     // 绘制底部边框
//     Console::draw_line(23, 0, CONSOLE_WIDTH, '=');
// }

void draw_level_deletion(const std::vector<Level> &levels) {
    Console::clear_screen();
    
    // 绘制标题
    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 20) / 2);
    std::cout << "DELETE CUSTOM LEVELS";
    
    // 绘制自定义关卡列表
    Console::move_to(3, 10);
    std::cout << "CUSTOM LEVEL LIST:";

    if(levels.size() == 4) {
        Console::move_to(5, 10);
        std::cout << "Empty";
    }
    else {
        for(int i = 4; i < levels.size() && i < 8; ++i) {
            Console::move_to(3 + i, 10);
            std::cout << "Level " << 5 + i;
        }
        if(levels.size() > 8) {
            Console::move_to(11, 10);
            std::cout << "(" << levels.size() - 8 << " Folded)";
        }
    }
    
    // 绘制警告信息
    Console::draw_box(10, 10, 60, 7);

    std::vector<std::string> text = {
        "WARNING: Deletion cannot be undone!",
        "Deleted levels will be permanently removed.",
        "",
        "Instructions:",
        "Enter level number to delete",
        "Enter 'a' to delete all,",
        "Enter 'b' to go back"
    };

    Console::draw_fig(11, 12, text);
    
    // 绘制底部边框
    Console::draw_line(21, 0, CONSOLE_WIDTH, '=');
    
    // 提示输入
    Console::move_to(22, 0);
    std::cout << "Input: ";

    std::cout << std::flush;
}

void draw_enter_level(const Level& level) {
    Console::clear_screen();
    
    // 绘制顶部边框和标题
    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 8) / 2);
    std::cout << "LEVEL " << level.id;
    
    Console::draw_box(3, 10, 60, 17);

    Console::move_to(4, 12);
    std::cout << "Description: ";
    auto des = split_string_to_fit(level.description, 56);
    Console::draw_fig(5, 14, des);
    int row = 5 +  des.size() - 1;

    Console::move_to(row += 2, 12);
    std::cout << "Inbox Elements:";
    Console::move_to(++row, 14);
    for(auto i : level.in) std::cout << i << ' ';

    Console::move_to(row += 2, 12);
    std::cout << "Outbox Elements:";
    Console::move_to(++row, 14);
    for(auto i : level.target) std::cout << i << ' ';

    Console::move_to(row += 2, 12);
    std::cout << "Available Open Area: " << level.area_num;

    Console::move_to(row += 2, 12);
    std::cout << "Available instructions:";
    Console::move_to(++row, 14);
    for(auto [name, x] : level.ava_cmd)
        std::cout << name << ' ';
    
    Console::move_to(22, 12);
    std::cout << "Enter '1' to load code from file, '2' to enter";
    Console::move_to(23, 12);
    std::cout << "code from keyboard, 'b' to go back";
    
    // 绘制底部边框
    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    Console::move_to(26, 0);
    std::cout << "Input: ";

    std::cout << std::flush;
}

void draw_enter_code(const Level &level) {
    Console::clear_screen();

    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 10) / 2);
    std::cout << "CODE INPUT";

    Console::move_to(2, 5);
    std::cout << "Available instructions:";
    Console::move_to(3, 7);
    for(auto [name, x] : level.ava_cmd)
        std::cout << name << ' ';
    Console::move_to(4, 5);
    std::cout << "Enter \"END\" to end the input.";
    Console::draw_line(5, 0, CONSOLE_WIDTH, '=');

    Console::move_to(6, 0);

    std::cout << std::flush;
}


// 代码显示 [l, r] 行
void draw_game_frame(const Simulator &sim, const Level &level, int &l, int &r) {
    Console::clear_screen();

    Console::draw_line(1, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 10) / 2);
    std::cout << "SIMULATION";

    // 关卡信息
    Console::draw_box(2, 4, 40, 4);
    Console::move_to(3, 6);
    std::cout << "Level " << level.id;
    Console::move_to(4, 6);
    std::cout << "Available Instructions:";
    std::string tmp;
    for(auto [name, x] : level.ava_cmd)
        tmp += " " + name;
    Console::draw_fig(5, 7, split_string_to_fit(tmp, 37));

    const std::vector<std::string> inbox = {
        "   +-----+",
        "   |     |",
        "   +-----+",
        "   |     |",
        "/ \\+-----+",
        " | |     |",
        " | +-----+",
        " | |     |",
        "   +-----+",
        "   |     |",
        "   +-----+",
        "   |     |",
        "   +-----+"
    };

    Console::move_to(8, 5);
    std::cout << "INBOX";
    Console::draw_fig(9, 0, inbox);

    const std::vector<int> &in = sim.get_inbox();
    for(int i = 0; i < 6; ++i) {
        if(i >= in.size()) 
            // Console::write_str(10 + i * 2, 5, 5, "X");
            continue;
        else
            Console::write_str(10 + i * 2, 5, 5, std::to_string(in[i]));
    }

    const std::vector<std::string> outbox = {
        "+-----+",
        "|     |",
        "+-----+",
        "|     |",
        "+-----+ |",
        "|     | |",
        "+-----+ |",
        "|     |\\ /",
        "+-----+",
        "|     |",
        "+-----+",
        "|     |",
        "+-----+"
    };

    Console::move_to(8, 41);
    std::cout << "OUTBOX";
    Console::draw_fig(9, 40, outbox);

    const std::vector<int> &out = sim.get_outbox();
    for(int i = 0; i < 6; ++i) {
        if(i >= out.size()) 
            // Console::write_str(10 + i * 2, 41, 5, "X");
            continue;
        else
            Console::write_str(10 + i * 2, 41, 5, std::to_string(*(out.rbegin() + i)));
    }

    Console::move_to(23, 3);
    std::cout << "OPEN AREA";

    const std::vector<int> &area = sim.get_area();
    if(area.empty()) std::cout << ": None";
    for(int i = 0; i < area.size(); ++i) {
        Console::draw_box(22, 13 + i * 6, 5, 1);
        int num = area[i];
        if(num != inf)
            Console::write_str(23, 14 + i * 6, 5, std::to_string(num));
    }

    std::vector<std::string> robot = {
        "+-----+",
        "|     |",
        "+-----+",
        " @   @ ",
        " ----- ",
        " |@ @| ",
        "   +   ",
        " /   \\",
        "  | |  "
    };

    int robot_x, robot_y, robot_pos = sim.get_robot_pos();
    if(robot_pos == -1) robot_x = 9, robot_y = 11;
    else if(robot_pos == -2) robot_x = 9, robot_y = 33;
    else robot_x = 14, robot_y = 13 + robot_pos * 6;
    Console::draw_fig(robot_x, robot_y, robot);
    int blk = sim.get_block();
    if(blk != inf)
        Console::write_str(robot_x + 1, robot_y + 1, 5, std::to_string(blk));

    Console::write_str(2, 50, 30, "CODE");
    Console::draw_line(3, 50, 30, '-');
    Console::draw_line(23, 50, 30, '-');
    Console::move_to(24, 51);
    std::cout << sim.get_cnt() << " instruction(s) executed";

    int cur = sim.get_cur();
    const std::vector<std::string> &cmd = sim.get_cmd();
    if(cur < l) l = cur, r = cur + 17;
    else if(cur > r) l = cur - 17, r = cur;
    
    for(int i = l, j = 0; i <= r && i <= cmd.size(); ++i, ++j) {
        Console::move_to(4 + j, 51);
        if(i == cur) std::cout << ">";
        else std::cout << " ";
        std::cout << std::setw(3) << i << " | " << cmd[i - 1];
    }


    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    Console::move_to(26, 0);

    std::cout << std::flush;
}

/*
  ______                                       _ 
 / ____/                                      | |
| (___   _   _   ____  ____  ___   ____  ____ | |
 \___ \ | | | | / __/ / __/ / _ \ / __/ / __/ |_|
 ____) || |_| || (__ | (__ |  __/ \__ \ \__ \  _
/_____/  \__,_| \___\ \___\ \___\ /___/ /___/ |_|
*/
void draw_level_success(const Pass_info &old, const Pass_info &nw, int id) {
    Console::clear_screen();

    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 6) / 2);
    std::cout << "RESULT";

    std::vector<std::string> success = {
        R"RAW(  ______                                       _ )RAW",
        R"RAW( / ____/                                      | |)RAW",
        R"RAW(| (___   _   _   ____  ____  ___   ____  ____ | |)RAW",
        R"RAW( \___ \ | | | | / __/ / __/ / _ \ / __/ / __/ |_|)RAW",
        R"RAW( ____) || |_| || (__ | (__ |  __/ \__ \ \__ \  _ )RAW",
        R"RAW(/_____/  \__,_| \___\ \___\ \___\ /___/ /___/ |_|)RAW"
    };

    Console::draw_fig(3, (CONSOLE_WIDTH - success[0].size()) / 2, success);

    Console::draw_box(12, 20, 40, 9);

    Console::move_to(14, 36);
    std::cout << "Level " << id;
    
    Console::move_to(16, 33);
    std::cout << "Code size: " << nw.min_code;
    if(!old.passed || nw.min_code < old.min_code) {
        Console::move_to(17, 33);
        std::cout << "(NEW RECORD!)";
    }

    Console::move_to(19,28);
    std::cout << "Instructions executed: " << nw.min_time;
    if(!old.passed || nw.min_code < old.min_code) {
        Console::move_to(20, 33);
        std::cout << "(NEW RECORD!)";
    }

    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    Console::move_to(26, 0);
    std::cout << "Press any key to return to the menu...";
    
    std::cout << std::flush;
}


/*
 ______         _   _ 
|  ____|       |_| | |
| |___   ____   _  | |
|  ___| / _  | | | | |
| |    | (_| | | | | |
|_|     \__,_| |_| |_|
*/

void draw_level_fail(const std::string &msg, const Simulator &sim, int id) {
    Console::clear_screen();

    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 6) / 2);
    std::cout << "RESULT";

    std::vector<std::string> fail = {
        R"RAW( ______         _   _ )RAW",
        R"RAW(|  ____|       |_| | |)RAW",
        R"RAW(| |___   ____   _  | |)RAW",
        R"RAW(|  ___| / _  | | | | |)RAW",
        R"RAW(| |    | (_| | | | | |)RAW",
        R"RAW(|_|     \__,_| |_| |_|)RAW"
    };

    Console::draw_fig(3, (CONSOLE_WIDTH - fail[0].size()) / 2, fail);

    Console::draw_box(12, 20, 40, 9);

    Console::move_to(14, 36);
    std::cout << "Level " << id;
    
    Console::write_str(15, 21, 40, msg);

    Console::move_to(17, 23);
    std::cout << "Inbox:  ";
    if(sim.get_inbox().empty())
        std::cout << "Empty";
    for(auto i : sim.get_inbox())
        std::cout << i << " ";

    Console::move_to(18, 23);
    std::cout << "Outbox: ";
    if(sim.get_outbox().empty())
        std::cout << "Empty";
    for(auto i : sim.get_outbox())
        std::cout << i << " ";

    Console::move_to(19, 23);
    std::cout << "Target: ";
    if(sim.get_target().empty())
        std::cout << "Empty";
    for(auto i : sim.get_target())
        std::cout << i << " ";

    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    Console::move_to(26, 0);
    std::cout << "Press any key to return to the menu...";
    
    std::cout << std::flush;
}


/*
 ______                      
|  ____|                     
| |___   _ __  _ __  ___   _ __
|  ___| | '__|| '__|/ _ \ | '__|
| |____ | |   | |  | (_) || |
|______||_|   |_|   \___/ |_|
*/
// void draw_level_error(const Simulator_Exception &err) {}

void view_help() {
    Console::clear_screen();
    
    // 绘制顶部边框和标题
    Console::draw_line(0, 0, CONSOLE_WIDTH, '=');
    Console::move_to(1, (CONSOLE_WIDTH - 10) / 2);
    std::cout << "GAME HELP";
    
    // 绘制帮助内容框
    Console::draw_box(3, 5, 70, 19);
    
    // 帮助内容标题
    Console::move_to(5, 7);
    std::cout << "Xiaoming's Robot - Help Guide";
    
    // 分割线
    Console::draw_line(6, 7, 67, '-');
    
    // 游戏概述
    std::vector<std::string> text = {
        "OVERVIEW:",
        "  You control a robot that must manipulate blocks with",
        "  numbers on them. The robot follows your program to",
        "  process blocks from INBOX and deliver results to OUTBOX."
    };

    Console::draw_fig(7, 7, text);
    
    // 分割线
    Console::draw_line(11, 7, 67, '-');
    
    // 指令集说明
    text = {
        "INSTRUCTION SET:",
        "  inbox        - Take first block from INBOX conveyor",
        "  outbox       - Place current block on OUTBOX conveyor",
        "  add X        - Add block at area X to current block",
        "  sub X        - Subtract block at area X from current block",
        "  copyto X     - Copy current block to area X",
        "  copyfrom X   - Copy block from area X to current block",
        "  jump X       - Jump to instruction X",
        "  jumpifzero X - Jump to instruction X if current block is 0"
    };
    
    Console::draw_fig(12, 7, text);
    // 分割线
    Console::draw_line(21, 7, 67, '-');
    
    // 分割线
    
    // 绘制底部边框
    Console::draw_line(25, 0, CONSOLE_WIDTH, '=');
    
    // 退出提示
    Console::move_to(26, 0);
    std::cout << "Press any key to go back...";

    std::cout << std::flush;
}