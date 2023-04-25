#ifndef CODEBLOCK_HPP
#define CODEBLOCK_HPP

#include <GUI/Component.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <vector>

namespace GUI {
class CodeBlock : public Component {
   public:
    typedef std::shared_ptr<CodeBlock> Ptr;

   public:
    CodeBlock(const FontHolder& fonts);

    void loadCode(const std::string& code);
    void setHighlight(const std::vector<int>& lineID);

    virtual bool isSelectable() const;
    virtual void handleEvent(const sf::Event& event);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void getLinePosition();

   private:
    std::vector<sf::Vector2f> mLinePositions;
    sf::Text mText;
    std::map<int, sf::RectangleShape> mHighlight;
};
}  // namespace GUI

#endif  // CODEBLOCK_HPP