#include <Constants.hpp>
#include <Utility.hpp>

#include <cmath>

void centerOrigin(sf::Sprite& sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                     std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                   std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::RectangleShape& rect) {
    sf::FloatRect bounds = rect.getLocalBounds();
    rect.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                   std::floor(bounds.top + bounds.height / 2.f));
}

sf::RectangleShape getLineShape(sf::Vector2f line, float thickness = 1.f) {
    float lineLength = sqrt(line.x * line.x + line.y * line.y);
    sf::RectangleShape rect(sf::Vector2f(lineLength, thickness));
    sf::FloatRect bounds = rect.getLocalBounds();
    rect.setOrigin(0, std::floor(bounds.top + bounds.height / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    rect.rotate(angle);
    return rect;
}

sf::ConvexShape getArrow(sf::Vector2f line, float thickness = 1.f) {
    float arrowEdge = thickness + 4.f;
    float arrowLength = sqrt(3.f) / 2.f * arrowEdge;
    float lineLength = sqrt(line.x * line.x + line.y * line.y);

    sf::ConvexShape arrow(4);
    arrow.setPoint(0, sf::Vector2f(2.f, thickness / 2.f));
    arrow.setPoint(1, sf::Vector2f(lineLength - arrowLength, thickness / 2.f));
    arrow.setPoint(2, sf::Vector2f(lineLength - arrowLength, -thickness / 2.f));
    arrow.setPoint(3, sf::Vector2f(2.f, -thickness / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    arrow.rotate(angle);
    return arrow;
}

sf::ConvexShape getArrowTip(sf::Vector2f line, float thickness = 1.f) {
    float lineLength = sqrt(line.x * line.x + line.y * line.y);
    float arrowEdge = thickness + 8.f;
    float arrowLength = sqrt(3.f) / 2.f * arrowEdge;

    sf::ConvexShape arrowTip(3);
    arrowTip.setPoint(0,
                      sf::Vector2f(lineLength - arrowLength, arrowEdge / 2.f));
    arrowTip.setPoint(1, sf::Vector2f(lineLength, 0.f));
    arrowTip.setPoint(2,
                      sf::Vector2f(lineLength - arrowLength, -arrowEdge / 2.f));

    float angle = atan2(line.y, line.x) / Constants::PI * 180.f;
    arrowTip.rotate(angle);
    return arrowTip;
}