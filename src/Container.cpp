#include <Container.hpp>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace GUI {

Container::Container() : mChildren(), mSelectedChild(-1), mActivatedChild(-1) {}

void Container::pack(Component::Ptr component) {
    mChildren.push_back(component);
}

void Container::reset() {
    if (hasActivation()) {
        mChildren[mActivatedChild]->deactivate();
        mChildren[mActivatedChild]->deselect();
    }
    mSelectedChild = mActivatedChild = -1;
}

bool Container::isSelectable() const {
    return false;
}

void Container::update(sf::Time dt) {
    for (int index = 0; index < mChildren.size(); index++) {
        mChildren[index]->update(dt);
    }
}

void Container::handleEvent(const sf::Event& event) {
    // If we have selected a child then give it events
    if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
        mChildren[mSelectedChild]->handleEvent(event);
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (hasSelection()) {
                if (hasActivation()) {
                    mChildren[mActivatedChild]->deactivate();
                    mChildren[mActivatedChild]->deselect();
                }
                mChildren[mSelectedChild]->activate();
                mActivatedChild = mSelectedChild;
            }
        }
    }
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const Component::Ptr& child : mChildren)
        target.draw(*child, states);
}

bool Container::hasSelection() const {
    return mSelectedChild >= 0;
}

bool Container::hasActivation() const {
    return mActivatedChild >= 0;
}

void Container::select(std::size_t index) {
    if (mChildren[index]->isSelectable() && !mChildren[index]->isActive()) {
        if (hasSelection() && !mChildren[mSelectedChild]->isActive())
            mChildren[mSelectedChild]->deselect();

        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void Container::updateSelect(sf::Vector2i point) {
    for (int index = 0; index < mChildren.size(); ++index) {
        if (mChildren[index]->isSelectable() && !mChildren[index]->isActive() &&
            mChildren[index]->contains(point)) {
            select(index);
            return;
        }
    }
    // no component is selected
    if (hasSelection() && !mChildren[mSelectedChild]->isActive()) {
        mChildren[mSelectedChild]->deselect();
        mSelectedChild = -1;
    }
}

}  // namespace GUI