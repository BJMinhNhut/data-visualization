#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>
#include <random>

namespace Random {
std::mt19937 rng(time(nullptr));

int get(int Min, int Max);

};  // namespace Random

#endif  // RANDOM_HPP