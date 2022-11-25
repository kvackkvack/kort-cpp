#include <iostream>
#include <vector>
#include <set>

#include "game.h"
#include "generate.h"


Generator::Generator() :
	max_height(0), all_cols{ {} },
	h_columns(new std::vector<std::vector<bool>>{ {} })
{

}


void Generator::gen_new_cols(unsigned int count) {
	for (int i = 0; i < count; i++) {
		auto new_h_columns = new std::vector<std::vector<bool>>{};
		std::vector<Column> h_cols;
		for (auto& col : *this->h_columns) {
			auto clone1 = std::vector<bool>(col);
			auto clone2 = std::vector<bool>(col);
			clone1.push_back(0);
			clone2.push_back(1);
			new_h_columns->push_back(clone1);
			new_h_columns->push_back(clone2);
			h_cols.push_back(from_vector(clone1));
			h_cols.push_back(from_vector(clone2));
		}
		this->h_columns = new_h_columns;
		this->all_cols.push_back(h_cols);
	}
}


std::vector<Column>& Generator::all_columns(unsigned int h) {
	this->gen_new_cols(h - this->max_height);
	return this->all_cols[h];
}


std::set<Game> Generator::all_games(unsigned int h, unsigned int w) {
	return all_games(h, w, [](Game* g) { return true; });
}

std::set<Game> Generator::all_even_games(unsigned int h, unsigned int w) {
	return this->all_games(h, w, [=](Game* g) { 
		return (count_my_cells(*g) == floor(h * w / 2));
	});
}

std::set<Game> Generator::all_games(unsigned int h, unsigned int w, std::function<bool(Game*)> filter) {
	auto& cols = this->all_columns(h);
	std::set<Game> all_games;
	std::set<Game> f_games;
	std::vector<std::vector<Column>> Ws{};
	Ws.push_back({});

	for (int x = 0; x < w; x++) {
		bool last_it = (x == w - 1);
		std::vector<std::vector<Column>> Qs{};
		for (auto& W : Ws) {
			for (auto& col : cols) {
				auto Q = std::vector<Column>(W);
				Q.push_back(col);
				if (last_it) {
					Game* g = new Game(Q);
					auto res = all_games.insert(*g);
					if (res.second && filter(g)) {
						f_games.insert(*g);
					}
				}
				else {
					Qs.push_back(Q);
				}
			}
		}
		Ws = Qs;
	}

	return f_games;
}