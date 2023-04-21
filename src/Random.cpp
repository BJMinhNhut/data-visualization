#include <Random.hpp>

int Random::get(int Min, int Max) {
    return std::uniform_int_distribution<int>(Min, Max)(rng);
}