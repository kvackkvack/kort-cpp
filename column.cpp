#include <iostream>
#include <bitset>

#include "column.h"


Column from_vector(std::vector<bool> cells) {
	std::bitset<16> res;
	auto cells_it = cells.begin();
	auto cells_left = true;
	int margin = 0;
	for (int i = 0; i < C; i++) {
		if (cells_it >= cells.end()) {
			cells_left = false;
		}
		if (cells_left) {
			res[15 - i] = *cells_it;
			cells_it++;
		}
		else {
			res[15 - i] = 0;
			margin += 1;
		}
	}
	// Copy margin into res[C:] bit for bit
	for (int i = 0; i < N; i++) {
		res[i] = (margin >> i) & 1U;
	}
	return static_cast<int>(res.to_ulong());
}

std::vector<bool> to_vector(Column x) {
	std::vector<bool> res;
	auto margin = x & N_MASK;
	for (int i = 15; i >= N + margin; i--) {
		res.push_back((x >> i) & 1U);
	}
	return res;
}


bool top_cell(Column x) {
	return (x >> 15) & 1U;
}

bool bot_cell(Column x) {
	auto margin = x & N_MASK;
	return (x >> (N + margin)) & 1U;
}


Column invert(Column x) {
	auto margin = x & N_MASK;
	auto invert_mask = ONES << (N + margin);
	return x ^ invert_mask;
}

Column play(Column x) {
	auto margin = x & N_MASK;
	auto set_zero_mask = ~(1UL << (N + margin));
	return (x + 1) & set_zero_mask;
}

Column append(Column x, bool cell) {
	auto margin = x & N_MASK;
	auto mask = 1UL << (N + margin - 1);
	if (cell) {
		return (x - 1) | mask;
	}
	else {
		return (x - 1) & ~mask;
	}
}


int num_cells(Column x) {
	return C - (x & N_MASK);
}


void print_bits(Column x) {
	std::cout << std::bitset<16>(x) << std::endl;
}
void print_column(Column x) {
	auto vector = to_vector(x);
	for (auto cell : vector) {
		std::cout << cell;
	}
}