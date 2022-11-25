#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <utility>
#include <random>
#include <functional>
#include <optional>
#include <unordered_set>
#include <map>
#include <set>

#include "main.h"
#include "game.h"
#include "test.h"
#include "random.h"



#ifndef TOTAL
void test_GoodToPlayInWhite(Randomiser* r, unsigned int n, unsigned int max_width, unsigned int max_height) {
	auto exs = find_least_example<std::pair<Game*, Game*>>(
		r, n, max_width, max_height,
		[r](unsigned int w, unsigned int h) -> std::pair<Game*, Game*> {
			auto W = r->random_uneven_columns(r->random_int(1, w - 1), 1, h);
			auto x = r->random_column_vector(r->random_int(0, h - 2));
			x.insert(x.begin(), 0);

			auto W1 = std::vector<Column>(W);
			W1.push_back(from_vector(x));

			auto W2 = W;
			x.push_back(0);
			W2.push_back(from_vector(x));

			return { new Game(W1), new Game(W2) };
		},
		[](const auto& games) -> bool {
			return winning(games.first) && !winning(games.second) && !(*invert(*games.second) == *games.second);
		},
		[](const auto& games) -> std::tuple<unsigned int, unsigned int, unsigned int> {
			return game_size(*games.first);
		},
		[](const auto& games) -> void {
			print_game(*games.first, "   ");
			print_game(*games.second, "     ");
			print_game(*invert(*games.second), "-----");
		},
		false
	);
}


void test_BadToPlayInBlack(Randomiser* r, unsigned int n, unsigned int max_width, unsigned int max_height) {
	auto exs = find_least_example<std::pair<Game*, Game*>>(
		r, n, max_width, max_height,
		[r](unsigned int w, unsigned int h) -> std::pair<Game*, Game*> {
			auto W = r->random_uneven_columns(r->random_int(1, w - 1), 1, h);
			auto x = r->random_column_vector(r->random_int(0, h - 1));
			x.insert(x.begin(), 1);

			auto W1 = std::vector<Column>(W);
			W1.push_back(from_vector(x));

			auto W2 = W;
			x.push_back(0);
			W2.push_back(from_vector(x));

			return { new Game(W1), new Game(W2) };
		},
		[](const auto& games) -> bool {
			return !winning(games.first) && winning(games.second);
		},
		[](const auto& games) -> std::tuple<unsigned int, unsigned int, unsigned int> {
			return game_size(*games.first);
		},
		[](const auto& games) -> void {
			print_game(*games.first, "   ");
			print_game(*games.second, "     ");
		},
		false
	);
}


void test_TwoPaddedEqual(Randomiser* r, unsigned int n, unsigned int max_width, unsigned int max_height) {
	auto exs = find_least_example<std::pair<Game*, Game*>>(
		r, n, max_width, max_height,
		[r](unsigned int w, unsigned int h) -> std::pair<Game*, Game*> {
			auto W = r->random_uneven_columns(r->random_int(0, w - 2), 1, h);
			auto x = r->random_column_vector(r->random_int(0, h - 2));
			auto y = r->random_column_vector(r->random_int(0, h - 2));
			auto a = r->random_int(0, 1);
			x.insert(x.begin(), a);
			x.push_back(0);
			y.insert(y.begin(), a);
			y.push_back(0);

			auto W1 = std::vector<Column>(W);
			auto x1 = std::vector<bool>(x);
			x1.push_back(0);
			W1.push_back(from_vector(x1));
			W1.push_back(from_vector(y));

			auto W2 = std::vector<Column>(W);
			auto y2 = std::vector<bool>(y);
			y2.push_back(0);
			W2.push_back(from_vector(x));
			W2.push_back(from_vector(y2));

			return { new Game(W1), new Game(W2) };
		},
		[](const auto& games) -> bool {
			return !winning(games.first) && winning(games.second);
		},
		[](const auto& games) -> std::tuple<unsigned int, unsigned int, unsigned int> {
			return game_size(*games.first);
		},
		[](const auto& games) -> void {
			print_game(*games.first, "   ");
			print_game(*games.second, "     ");
		},
		true
	);
}

void test_TwoPaddedBetterInBlack(Randomiser* r, unsigned int n, unsigned int max_width, unsigned int max_height) {
	auto exs = find_least_example<std::pair<Game*, Game*>>(
		r, n, max_width, max_height,
		[r](unsigned int w, unsigned int h) -> std::pair<Game*, Game*> {
			auto W = r->random_uneven_columns(r->random_int(0, w - 2), 1, h);
			auto x = r->random_column_vector(r->random_int(0, h - 2));
			auto y = r->random_column_vector(r->random_int(0, h - 2));
			x.insert(x.begin(), 1);
			x.push_back(0);
			y.insert(y.begin(), 1);
			y.push_back(0);

			auto W1 = std::vector<Column>(W);
			auto x1 = std::vector<bool>(x);
			x1.push_back(0);
			W1.push_back(from_vector(x1));
			W1.push_back(from_vector(y));

			auto W2 = std::vector<Column>(W);
			auto y2 = std::vector<bool>(y);
			y2.push_back(0);
			W2.push_back(from_vector(x));
			W2.push_back(from_vector(y2));

			return { new Game(W1), new Game(W2) };
		},
		[](const auto& games) -> bool {
			return !winning(games.first) && winning(games.second);
		},
		[](const auto& games) -> std::tuple<unsigned int, unsigned int, unsigned int> {
			return game_size(*games.first);
		},
		[](const auto& games) -> void {
			print_game(*games.first, "   ");
			print_game(*games.second, "     ");
		},
		true
	);
}
#endif


//#define PRINT_BITS
int main() {
	Generator* gen = new Generator();
	/* for (int w = 1; w < 8; w++) {
		auto gs = gen->all_even_games(3, w);
		std::cout << "--- " << gs.size() << std::endl;
		for (auto& g : gs) {
			print_game(g, "   ");
		}
	} */

	Randomiser* r = new Randomiser();
	test_GoodToPlayInWhite(r, 1000, 5, 5);

	/* Game* g = new Game({
		from_vector({ 1, 1, 0 }),
		from_vector({ 1, 0 }),
		from_vector({ 0, 1, 1, 0, 0 }),
		from_vector({ 0, 1, 1 })
	});
	std::vector<std::vector<std::pair<bool, Game*>>> paths;
	bool win;
	std::tie(win, paths) = winning(g);
	print_game(*g, "");
	if (win) {
		std::cout << "Winning!" << std::endl;
	}
	else {
		std::cout << "Losing." << std::endl;
	}
	for (auto& path : paths) {
		std::cout << "--- ONE PATH ---" << std::endl;
		for (auto phase : path) {
			auto& game = phase.second;
			if (phase.first) {
				for (auto& col : game->columns) {
					col = invert(col);
				}
			}
			print_game(*game, "");
		}
	} */
}