#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

namespace Constants {
// Drawing properties
float const NODE_SIZE = 40.f;
float const NODE_DISTANCE = 80.f;

int const NODE_MAXVALUE = 99;
int const NODE_MINVALUE = 0;
int const LIST_MAXSIZE = 10;
int const ARRAY_MAXSIZE = 10;

// Math
float const PI = 3.14159265;

// Color
sf::Color const mBlack(66, 66, 66);
sf::Color const BlueDark(13, 71, 161);
sf::Color const WhiteDisplay(sf::Color::White);
sf::Color const WhiteUI(245, 245, 245);
sf::Color const GrayBorder(189, 189, 189);
sf::Color const YellowLight(255, 245, 157);
sf::Color const RedDark(183, 28, 28);

sf::Color const BlackUI(69, 90, 100);
sf::Color const BlackDisplay(55, 71, 79);
sf::Color const RedLight(229, 115, 115);
sf::Color const BlueLight(100, 181, 246);
sf::Color const TealDark(0, 105, 92);
sf::Color const BlueGrayBorder(96, 125, 139);

// Input
int const INPUT_MAX_LENGTH = 34;
int const INPUT_WRAP_LENGTH = 17;
sf::Time const INPUT_CURSOR_LIFE(sf::milliseconds(400));
}  // namespace Constants

#endif  // CONSTANTS_HPP