#pragma once

#include <vector>
#include <string>
#include "column.h"

constexpr auto M = 8;

class Game {
public:
	Column columns[M];
	Game();
	Game(const Game&);
	Game(std::vector<Column>);
	Game(std::vector<Column>, bool);
};
bool operator<(const Game&, const Game&);
bool operator==(const Game&, const Game&);

template<>
struct std::hash<Game>;

int height(const Game&);
int width(const Game&);
int num_cells(const Game&);
int count_my_cells(Game&);
Game* invert(const Game&);

Game& add_column(const Game&, const Column&);

void print_game(const Game&, std::string);
void print_bits(const Game&);


#ifdef TOTAL
std::pair<bool, std::vector<std::vector<std::pair<bool, Game*>>>>
#else
bool
#endif
winning_(Game*, bool, int, bool);

#ifdef TOTAL
std::pair<bool, std::vector<std::vector<std::pair<bool, Game*>>>>
#else
bool
#endif
winning(Game*);