#include <Random.hpp>
#include <ctime>
#include <random>

std::mt19937 rng(time(nullptr));

int Random::get(int Min, int Max) {
	return std::uniform_int_distribution<int>(Min, Max)(rng);
}