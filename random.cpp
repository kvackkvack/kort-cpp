#include <random>

#include "random.h"
#include "generate.h"


Randomiser::Randomiser() : Randomiser(new Generator)
{}

Randomiser::Randomiser(Generator* gen) : bool_dist(0, 1), gen(gen) 
{
	std::random_device r;
	this->rd = new std::default_random_engine(r());
}


int Randomiser::random_int(int a, int b) {
	std::uniform_int_distribution<int> dist(a, b);
	return dist(*this->rd);
}


// Robert Floyd method
std::set<unsigned int>& Randomiser::random_comb(unsigned int n, unsigned int m) {
	std::set<unsigned int> res;
	for (int j = n - m; j < n; j++) {
		std::uniform_int_distribution<unsigned int> dist(0, j);
		auto t = dist(*this->rd);
		if (res.count(t)) {
			res.insert(j);
		}
		else {
			res.insert(t);
		}
	}
	return res;
}

template<class T>
std::vector<T>& Randomiser::random_submulti(
	const std::vector<T>& items,
	unsigned int m
) {
	const unsigned int& n = items.size();
	std::vector<T> res;
	std::set<unsigned int> star_indexes = this->random_comb(n + m - 1, m);
	unsigned int j = 0;
	unsigned int prev_next_i = 0;
	for (auto& i : star_indexes) {
		j += i - prev_next_i;
		res.push_back(items[j]);
		prev_next_i = i + 1;
	}
	return res;
}


std::vector<bool> Randomiser::random_column_vector(unsigned int h) {
	std::vector<bool> column{};
	for (int j = 0; j < h; j++) {
		column.push_back(bool_dist(*this->rd));
	}
	return column;
}

Column Randomiser::random_column(unsigned int h) {
	return from_vector(random_column_vector(h));
}

std::vector<Column> Randomiser::random_columns(unsigned int width, unsigned int height) {
	std::vector<Column> columns{};
	for (int i = 0; i < width; i++) {
		columns.push_back(this->random_column(height));
	}
	return columns;
}

std::vector<Column> Randomiser::random_uneven_columns(unsigned int width, unsigned int min_height, unsigned int max_height) {
	std::uniform_int_distribution<unsigned int> height_dist(min_height, max_height);
	std::vector<Column> columns{};
	for (int i = 0; i < width; i++) {
		columns.push_back(this->random_column(height_dist(*this->rd)));
	}
	return columns;
}


Game* Randomiser::fair_random_rect_game(unsigned int width, unsigned int height) {
	// TODO: we need to make sure these are sorted
	auto& cols = random_submulti(this->gen->all_columns(height), width);
	return new Game(cols, true);
}

Game* Randomiser::random_rect_game(unsigned int width, unsigned int height) {
	return new Game(random_columns(width, height));
}

Game* Randomiser::random_game(unsigned int width, unsigned int min_height, unsigned int max_height) {
	return new Game(random_uneven_columns(width, min_height, max_height));
}


/* void Randomiser::gen_new_cols(unsigned int to_height) {
	if (to_height > this->max_height) {
		std::vector<std::vector<Column>> got_cols;
		std::tie(got_cols, this->h_columns) = all_columns(to_height - this->max_height, this->h_columns);
		for (auto& h_cols : got_cols) {
			std::sort(h_cols.begin(), h_cols.end(), std::greater<Column>());
			this->all_cols.push_back(h_cols);
		}
		this->max_height = to_height;
	}
} */