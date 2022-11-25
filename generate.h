#pragma once

#include <functional>
#include <vector>
#include <set>

#include "game.h"


class Generator {
public:
	Generator();

	std::vector<Column>& all_columns(unsigned int);

	std::set<Game> all_games(unsigned int, unsigned int);
	std::set<Game> all_even_games(unsigned int, unsigned int);
	std::set<Game> all_games(unsigned int, unsigned int, std::function<bool(Game*)> filter);

private:
	unsigned int max_height;
	std::vector<std::vector<Column>> all_cols;
	std::vector<std::vector<bool>>* h_columns;

	void gen_new_cols(unsigned int);
};