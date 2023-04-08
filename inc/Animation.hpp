#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

class Animation {
    enum Type { Highlight, Move, TypeCount };
    sf::Time duration;
    sf::Vector2f moveVector;
    unsigned int codeID;
    unsigned int lineID;
};

#endif  // ANIMATION_HPP    