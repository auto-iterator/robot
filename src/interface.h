#pragma once

#include "level.h"
#include "console.h"
#include "simulator.h"

void draw_start_page();
void draw_level_selection(const std::vector<Level>&, const std::vector<Pass_info>&);
// void draw_select_level_creation_method();
// void draw_level_creation();
void draw_import_from_file();
void draw_level_deletion(const std::vector<Level>&);
void draw_enter_level(const Level&);
void draw_enter_code(const Level&);
void draw_game_frame(const Simulator&, const Level&, int&, int&); 
void draw_level_success(const Pass_info&, const Pass_info&, int);
void draw_level_fail(const std::string&, const Simulator&, int);
// void draw_level_error(const Simulator_Exception&);
void view_help();