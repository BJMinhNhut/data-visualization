#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <GUI/Component.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

namespace GUI {
class ProgressBar : public Component {
   public:
    typedef std::shared_ptr<ProgressBar> Ptr;

   public:
    ProgressBar(const sf::Vector2f& size);

    virtual bool isSelectable() const;

    void setLength(const int& length);
    void setProgress(const int& progress);

    virtual void handleEvent(const sf::Event& event);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void updateBar();

   private:
    const sf::Vector2f mSize;
    int mLength;
    int mProgress;

    sf::RectangleShape mBar;
};
}  // namespace GUI

#endif  // PROGRESSBAR_HPP