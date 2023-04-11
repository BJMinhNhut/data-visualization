#include <Component.hpp>

namespace GUI {

Component::Component() : mIsSelected(false), mIsActive(false) {}

Component::~Component() {}

bool Component::isSelected() const {
    return mIsSelected;
}

void Component::select() {
    mIsSelected = true;
}

void Component::deselect() {
    mIsSelected = false;
}

bool Component::isActive() const {
    return mIsActive;
}

void Component::activate() {
    mIsActive = true;
}

void Component::deactivate() {
    mIsActive = false;
}

bool Component::contains(sf::Vector2i point) const {
    return false;
}

}  // namespace GUI