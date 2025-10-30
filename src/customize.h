#ifndef CUSTOMIZE_H
#define CUSTOMIZE_H

#include<fstream>
#include"command.h"
#include"level.h"

template<typename T>
int read(std::ifstream&, T&);

int read_level(std::ifstream&, Level&);

void write_level(std::ofstream&, Level&);

#endif