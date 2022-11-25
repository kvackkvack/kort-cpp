#pragma once

#include <vector>


constexpr auto C = 12;
constexpr auto N = 4;
constexpr unsigned int ONES = 0b1111111111111111;
constexpr unsigned int ZEROS = 0b0000000000000000;
constexpr unsigned int N_MASK = ONES >> C;

typedef unsigned int Column;

Column from_vector(std::vector<bool>);
std::vector<bool> to_vector(Column);

bool top_cell(Column);
bool bot_cell(Column);

Column invert(Column);
Column play(Column);
Column append(Column, bool);

int num_cells(Column);

void print_bits(Column);
void print_column(Column);