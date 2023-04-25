#include <Constants.hpp>
#include <GUI/ProgressBar.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {
ProgressBar::ProgressBar(const sf::Vector2f& size)
    : mLength(1), mProgress(0), mSize(size), mBar() {
    mBar.setFillColor(Constants::mBlue);
    updateBar();
}

bool ProgressBar::isSelectable() const {
    return false;
}

void ProgressBar::setLength(const int& length) {
    mLength = length;
    mProgress = std::min(mLength, mProgress);
    updateBar();
}

void ProgressBar::setProgress(const int& progress) {
    mProgress = progress;
    updateBar();
}

void ProgressBar::handleEvent(const sf::Event& event) {}

void ProgressBar::updateBar() {
    float width =
        static_cast<float>(mProgress) / static_cast<float>(mLength) * mSize.x;
    mBar.setSize(sf::Vector2f(width, mSize.y));
}

void ProgressBar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mBar, states);
}

}  // namespace GUI