#ifndef COLORHOLDER_HPP
#define COLORHOLDER_HPP

#include <SFML/Graphics/Color.hpp>

#include <cassert>
#include <map>
#include <memory>
#include <string>

namespace Colors {
enum ID {
    Text,
    UIPrimary,
    UISecondary,
    UIBorder,
    Blue,
    Red,
    Highlight,
};
}

class ColorHolder {
   public:
    void load(Colors::ID id, const sf::Color& color);

    const sf::Color& get(Colors::ID id) const;

   private:
    void insertColor(Colors::ID id, std::unique_ptr<sf::Color> color);

   private:
    std::map<Colors::ID, std::unique_ptr<sf::Color>> mColorMap;
};

#endif  // COLORHOLDER_HPP