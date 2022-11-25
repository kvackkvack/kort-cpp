#include <optional>
#include <functional>

#include "test.h"
#include "game.h"


std::tuple<unsigned int, unsigned int, unsigned int> game_size(const Game& game) {
	return { num_cells(game), width(game), height(game) };
}


/*
std::optional<std::pair<Game*, Game*>> least_counter_example{};
int least_counter_example_size = -1;
for (int i = 0; i < n;) {
	int w = gen_width();
	std::vector<Column> columns{};
	for (int i = 0; i < w; i++) {
		auto col = random_column_vector(gen_height());
		col.push_back(1); // Make sure this is a pass for the other player
		columns.push_back(from_vector(col));
	}
	auto col = random_column_vector(gen_height());
	col.insert(col.begin(), 0);
	col.push_back(1);
	auto columns1 = std::vector<Column>(columns);
	columns1.push_back(from_vector(col));
	Game* g1 = new Game(columns1);

	if (winning(g1)) {
		col.push_back(0);
		columns.push_back(from_vector(col));
		Game* g2 = new Game(columns);
		i++;
		if (!winning(g2)) {
			int size = num_cells(*g1);
			if (!least_counter_example || size < least_counter_example_size) {
				least_counter_example_size = size;
				least_counter_example = std::make_optional(std::make_pair(g1, g2));
			}
		}
	}
}
if (least_counter_example) {
	std::cout << "!!! Counterexample !!!" << std::endl;
	print_game(*least_counter_example->first, "");
	print_game(*least_counter_example->second, "");
}
else {
	std::cout << "ok" << std::endl;
} */