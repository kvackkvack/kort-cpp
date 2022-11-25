#pragma once

#include <vector>
#include <set>
#include <optional>
#include <random>

#include "column.h"
#include "game.h"
#include "generate.h"


class Randomiser {
public:
	std::default_random_engine* rd;
	std::uniform_int_distribution<unsigned int> bool_dist;
	Generator* gen;

	Randomiser();
	Randomiser(Generator*);

	int random_int(int, int);

	std::set<unsigned int>& random_comb(unsigned int, unsigned int);
	template<class T> std::vector<T>& random_submulti(const std::vector<T>&, unsigned int);

	std::vector<bool> random_column_vector(unsigned int);
	Column random_column(unsigned int);
	std::vector<Column> random_columns(unsigned int, unsigned int);
	std::vector<Column> random_uneven_columns(unsigned int, unsigned int, unsigned int);

	Game* fair_random_rect_game(unsigned int, unsigned int);
	Game* random_rect_game(unsigned int, unsigned int);
	Game* random_game(unsigned int, unsigned int, unsigned int);
};