#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <Component.hpp>

#include <memory>
#include <vector>

namespace GUI {
class Container : public Component {
   public:
    typedef std::shared_ptr<Container> Ptr;

   public:
    Container();

    void pack(Component::Ptr component);

    void reset();

    virtual bool isSelectable() const;
    virtual void update(sf::Time dt);
    virtual void handleEvent(const sf::Event& event);

    void updateSelect(sf::Vector2i point);

   private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool hasSelection() const;
    bool hasActivation() const;
    void select(std::size_t index);

   private:
    std::vector<Component::Ptr> mChildren;
    int mSelectedChild;
    int mActivatedChild;
};
}  // namespace GUI

#endif  // CONTAINER_HPP