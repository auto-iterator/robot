#pragma once
#include<fstream>
#include"command.h"
#include"level.h"

Level read_level(std::ifstream&, int);

void write_level(std::ofstream&, Level&);
