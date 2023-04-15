#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

namespace Constants {
// Drawing properties
float const NODE_SIZE = 40.f;
float const NODE_DISTANCE = 80.f;

int const NODE_MAXVALUE = 99;
int const NODE_MINVALUE = 1;
unsigned int const LIST_MAXSIZE = 10;

// Math
float const PI = 3.14159265;

// Color
sf::Color const mBlack(66, 66, 66);

// Input
int const INPUT_MAX_LENGTH = 9;
sf::Time const INPUT_CURSOR_LIFE(sf::milliseconds(400));

}  // namespace Constants

#endif  // CONSTANTS_HPP