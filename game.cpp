#include <iostream>
#include <algorithm>
#include <map>
#include <bitset>

#include "game.h"


Game::Game() {
	for (int i = 0; i < M; i++) {
		this->columns[i] = from_vector({});
	}
}
Game::Game(const Game& game) {
	for (int i = 0; i < M; i++) {
		this->columns[i] = game.columns[i];
	}
}
Game::Game(std::vector<Column> columns, bool bypass) {
	for (int i = columns.size(); i < M; i++) {
		columns.push_back(from_vector({}));
	}
	if (!bypass) {
		std::sort(columns.begin(), columns.end(), std::greater<Column>());
	}
	for (int i = 0; i < M; i++) {
		this->columns[i] = columns[i];
	}
}
Game::Game(std::vector<Column> columns) : Game(columns, false) { }


bool operator<(const Game& g1, const Game& g2) {
	for (int i = 0; i < M; i++) {
		if (g1.columns[i] < g2.columns[i]) {
			return true; // LT
		}
		else if (g1.columns[i] > g2.columns[i]) {
			return false; // GT
		}
	}
	return false; // equal
}

bool operator==(const Game& g1, const Game& g2) {
	for (int i = 0; i < M; i++) {
		if (g1.columns[i] != g2.columns[i]) {
			return false;
		}
	}
	return true;
}

template<>
struct std::hash<Game> {
	std::hash<Column> hasher;
	auto operator()(const Game& game) const {
		size_t result = 0;
		for (size_t i = 0; i < M; i++) {
			result = result * 31 + hasher(game.columns[i]);
		}
		return result;
	}
};



int width(const Game& g) {
	int w = 0;
	for (int i = 0; i < M; i++) {
		if ((g.columns[i] & N_MASK) < C) {
			w += 1;
		}
	}
	return w;
}

// max . map num_cells
int height(const Game& g) {
	int h = 0;
	for (int i = 0; i < M; i++) {
		int local_h = num_cells(g.columns[i]);
		if (local_h > h) {
			h = local_h;
		}
	}
	return h;
}

// sum . map num_cells
int num_cells(const Game& g) {
	int sum = 0;
	for (int i = 0; i < M; i++) {
		// print_column(g.columns[i]);
		sum += num_cells(g.columns[i]);
	}
	return sum;
}

// sum . map (filter (== 0) . cells)
int count_my_cells(Game& g) {
	int n = 0;
	for (int i = 0; i < M; i++) {
		if ((g.columns[i] & N_MASK) < C) {
			std::bitset<16> bs((g.columns[i] >> N) << N);
			n += bs.count();
		}
	}
	return n;
}


Game* invert(const Game& g) {
	std::vector<Column> cols;
	for (int i = 0; i < M; i++) {
		auto col = g.columns[i];
		if ((col & N_MASK) >= C) {
			cols.push_back(invert(col));
		}
	}
	return new Game(cols);
}

Game& add_column(const Game& g, const Column& new_col) {
	int replace_i = -1;
	for (int i = 0; i < M; i++) {
		auto col = g.columns[i];
		if ((col & N_MASK) >= C) {
			replace_i = i;
			break;
		}
	}
	if (replace_i == -1) {
		throw std::exception("Tried to add column to a Game that is already full");
	}
	else {
		auto h = new Game();
		int k = 0;
		bool to_add = true;
		for (int i = 0; i < M; i++) {
			if (k == replace_i) {
				k++;
			}
			auto& col = g.columns[k];
			if (to_add && new_col >= col) {
				h->columns[i] = new_col;
				to_add = false;
			}
			else {
				h->columns[i] = col;
				k++;
			}
		}
		if (to_add) {
			h->columns[M - 1] = new_col;
		}
		return *h;
	}
}


#ifdef TOTAL
std::pair<bool, std::vector<std::vector<std::pair<bool, Game*>>>>
#else
std::map<std::pair<Game&, bool>, bool> cache{};

bool
#endif
winning_(Game* g, bool player, int depth, bool can_pass) {
#ifdef LOG
	std::string indent;
	indent = std::string(2 * depth, ' ');
	std::cout << indent << "---  @" << depth << " as " << player << std::endl;
	print_game(*g, indent);
#endif

#ifndef TOTAL
	if (cache.count({ *g, player })) {
		auto win = cache.at({ *g, player });
#ifdef LOG
		std::cout << indent << "Position is cached; winning? = " << win << std::endl;
#endif
		return win;
	}
#endif

	bool zero_tops = true;
	bool only_one_lone_top = false;
	for (auto col : g->columns) {
		if ((col & N_MASK) >= C) {
			continue;
		}
		if (top_cell(col) ^ player) {
			if (only_one_lone_top) {
				only_one_lone_top = false;
				break;
			}
			else {
				zero_tops = false;
				if ((col & N_MASK) == C - 1) {
					only_one_lone_top = true;
				}
				else {
					break;
				}
			}
		}
	}
	if (zero_tops) {
#ifdef LOG
		std::cout << indent << "Primitively winning" << std::endl;
#endif
#ifdef TOTAL
		return { true, {{}} };
#else
		return true;
#endif
	}
	if (only_one_lone_top) {
#ifdef LOG
		std::cout << indent << "Primitively winning after one move" << std::endl;
#endif
#ifdef TOTAL
		return { true, {{}} };
#else
		return true;
#endif
	}


#ifdef TOTAL
	std::vector<std::vector<std::pair<bool, Game*>>> winning_paths{};
	std::vector<std::vector<std::pair<bool, Game*>>> losing_paths{};
#endif
	bool winnable = false;
	bool had_moves = false;
	for (int i = 0; i < M; i++) {
		auto col = g->columns[i];
		auto margin = col & N_MASK;
		if (margin < C && (bot_cell(col) ^ player)) {
			had_moves = true;
			auto h = new Game();
			auto new_col = play(col);
			bool to_add = true;
			int k = 0;
			for (int j = 0; j < M; j++) {
				if (j == i) {
					continue;
				}
				if (to_add && (new_col >= g->columns[j])) {
					h->columns[k] = new_col;
					h->columns[k + 1] = g->columns[j];
					k++;
					to_add = false;
				}
				else {
					h->columns[k] = g->columns[j];
				}
				k++;
			}
			if (to_add) {
				h->columns[k] = new_col;
			}

			auto was_last_cell = (margin == C - 1);
#ifdef TOTAL
			auto new_player = player ^ !was_last_cell;
			auto res = winning_(new Game(*h), new_player, depth + 1, was_last_cell);
			auto win = res.first ^ !was_last_cell;
			winnable |= (bool)win;
			for (auto path : res.second) {
				path.push_back({ new_player, h });
				if (win) {
					winning_paths.push_back(path);
				}
				else if (!winnable) {
					losing_paths.push_back(path);
				}
			}
#else
			// if (winning_(h, player ^ !was_last_cell, depth + 1, was_last_cell) ^ !was_last_cell) {
			if (!winning_(h, !player, depth + 1, false)) {
#ifdef LOG
				std::cout << indent << "-> winning" << std::endl;
#endif
				cache.insert({ { *new Game(*g), player }, true });
				return true;
			}
#endif
		}
	}
	if (can_pass || !had_moves) {
#ifdef LOG
		std::cout << indent << "Passing..." << std::endl;
#endif
#ifdef TOTAL
		auto res = winning_(new Game(*g), !player, depth + 1, false);
		auto win = !res.first;
		winnable |= win;
		for (auto path : res.second) {
			path.push_back({ !player, g });
			if (win) {
				winning_paths.push_back(path);
			}
			else if (!winnable) {
				losing_paths.push_back(path);
			}
		}
#else
		auto win = !winning_(g, !player, depth + 1, false);
		cache.insert({ { *new Game(*g), player }, win });
		return win;
#endif
	}
#ifdef LOG
	std::cout << indent << "-> losing" << std::endl;
#endif
#ifdef TOTAL
#ifdef LOG
	std::cout << indent << "Paths:" << std::endl;
	std::cout << indent << " * Losing:" << std::endl;
	for (auto& path : losing_paths) {
		std::string indent2 = indent + "    ";
		for (auto game : path) {
			print_game(*game.second, indent2);
			indent2 += "  ";
		}
	}
	std::cout << indent << " * Winning:" << std::endl;
	for (auto& path : winning_paths) {
		std::string indent2 = indent + "    ";
		for (auto game : path) {
			print_game(*game.second, indent2);
			indent2 += "  ";
		}
	}
#endif
	if (winnable) {
		return { true, winning_paths };
	}
	else {
		return { false, losing_paths };
	}
#
#else
	cache.insert({ { *new Game(*g), player }, false });
	return false;
#endif
}

#ifdef TOTAL
std::pair<bool, std::vector<std::vector<std::pair<bool, Game*>>>>
#else
bool
#endif
winning(Game* g) {
	auto res = winning_(g, 0, 0, false);
#ifdef TOTAL
	for (auto& path : res.second) {
		path.push_back({ 0, g });
	}
#endif
	return res;
}


void print_game(const Game& g, std::string indent) {
	try {
		int max_col_height = 0;
		int width = 0;
		for (int i = 0; i < M; i++) {
			auto col = g.columns[i];
			auto h = C - (col & N_MASK);
			if (h > max_col_height) {
				max_col_height = h;
			}
			if (h > 0) {
				width++;
			}
		}

		int k = 0;
		for (int i = 0; i < M; i++) {
			auto col = g.columns[i];
			auto h = 12u - (col & N_MASK);
			if (h <= 0) {
#ifdef PRINT_BITS
				std::cout << indent << std::string(max_col_height + 5, ' ');
				print_bits(col);
#endif
				continue;
			}
			std::cout << indent;
			if (k == 0) {
				if (k == width - 1) {
					std::cout << "[ ";
				}
				else {
					std::cout << "/ ";
				}
			}
			else if (k == width - 1) {
				std::cout << "\\ ";
			}
			else {
				std::cout << "| ";
			}
			std::cout << std::string(std::max(0u, max_col_height - h), ' ');
			print_column(col);
			if (i == 0) {
				if (k == width - 1) {
					std::cout << " ]";
				}
				else {
					std::cout << " \\";
				}
			}
			else if (k == width - 1) {
				std::cout << " /";
			}
			else {
				std::cout << " |";
			}
#ifdef PRINT_BITS
			std::cout << " ";
			print_bits(col);
#else
			std::cout << std::endl;
#endif
			k++;
		}
	}
	catch (...) {
		std::cout << "<<< (game failed to print)" << std::endl;
		print_bits(g);
		std::cout << ">>>" << std::endl;
	}
}

void print_bits(const Game& game) {
	for (int i = 0; i < M; i++) {
		print_bits(game.columns[i]);
	}
}