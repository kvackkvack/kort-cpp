#pragma once

#include <iostream>
#include <vector>

#include "game.h"
#include "random.h"


template<class T>
std::vector<T> find_least_example(
	Randomiser* r, unsigned int N, unsigned int init_w, unsigned int init_h,
	std::function<T(unsigned int, unsigned int)> get_ex,
	bool(*valid)(const T&),
	std::tuple<unsigned int, unsigned int, unsigned int>(*find_size)(const T&),
	void(*print_ex)(const T&), bool do_print
);

template<class T>
std::vector<T> find_least_example(
	Randomiser* r, unsigned int N, unsigned int init_w, unsigned int init_h,
	std::function<T(unsigned int, unsigned int)> get_ex,
	bool(*valid)(const T&),
	std::tuple<unsigned int, unsigned int, unsigned int>(*find_size)(const T&),
	void(*print_ex)(const T&), bool do_print
) {
	std::vector<T> least_counter_examples{};
	unsigned int least_size = -1,
		         least_width = init_w,
		         least_height = init_h;
	for (int i = 0; i < N; i++) {
		auto ex = get_ex(least_width, least_height);
		if (valid(ex)) {
			std::cout << "!!! COUNTEREXAMPLE !!!" << std::endl;
			print_ex(ex);
			auto sizes = find_size(ex);
			auto [size, width, height] = sizes;
			if (least_size == -1 || size <= least_size || width <= least_width || height <= least_height) {
				if (size < least_size) {
					least_counter_examples.clear();
				}
				least_counter_examples.push_back(ex);
				std::tie(least_size, least_width, least_height) = sizes;
			}
		}
		else if(do_print) {
			auto c = r->random_int(0, 100);
			if (c <= 5) {
				std::cout << "Proven non-counterexample" << std::endl;
				print_ex(ex);
			}
		}
		if (i % 500 == 0) {
			std::cout << "(" << i << "/" << N << ")" << std::endl;
		}
	}

	if (least_counter_examples.empty()) {
		std::cout << "Passed!" << std::endl;
	}
	else {
		std::cout << "Least counter examples:  " << std::endl;
		for (auto& ex : least_counter_examples) {
			print_ex(ex);
		}
	}

	return least_counter_examples;
}


std::tuple<unsigned int, unsigned int, unsigned int> game_size(const Game& game);
