#pragma once
#include<fstream>
#include"command.h"
#include"level.h"

void read_level(std::ifstream&, Level&);

void write_level(std::ofstream&, Level&);

void read_completion(std::ifstream&, std::vector<int>&);

void write_completion(std::ofstream&, std::vector<int>&);

void read_customized_level_path(std::ifstream&, std::vector<std::pair<int, std::string>>&);

void write_customized_level_path(std::ofstream&, std::vector<std::pair<int, std::string>>&);