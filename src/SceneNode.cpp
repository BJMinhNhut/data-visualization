#include <SceneNode.hpp>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

SceneNode::SceneNode()
    : mChildren(),
      mParent(nullptr),
      targetPosition(0.f, 0.f),
      targetScale(1.f, 1.f) {}

void SceneNode::attachChild(Ptr child) {
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(SceneNode* node) {
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                              [&](Ptr& p) { return p.get() == node; });
    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::update(sf::Time dt) {
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt) {
    // Do nothing as default
    // std::cerr << std::setprecision(3) << std::fixed << '\n';
    sf::Vector2f deltaPosition = (targetPosition - getPosition()) * 0.1f;
    move(deltaPosition);
    // std::cerr << targetPosition.x << ' ' << getPosition().x << '\n';

    sf::Vector2f deltaScale = (targetScale - getScale()) * 0.1f;
    // std::cerr << std::setprecision(3) << std::fixed << '\n';
    // std::cerr << targetScale.x << ' ' << getScale().x << ' ' << deltaScale.x
    //           << '\n';
    setScale(getScale() + deltaScale);
}

void SceneNode::updateChildren(sf::Time dt) {
    for (Ptr& child : mChildren) {
        child->update(dt);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target,
                            sf::RenderStates states) const {}

void SceneNode::drawChildren(sf::RenderTarget& target,
                             sf::RenderStates states) const {
    for (const Ptr& child : mChildren) {
        child->draw(target, states);
    }
}

void SceneNode::setTargetPosition(sf::Vector2f position, bool teleport) {
    targetPosition = position;
    if (teleport)
        setPosition(targetPosition);
}

void SceneNode::setTargetPosition(float pX, float pY, bool teleport) {
    targetPosition = sf::Vector2f(pX, pY);
    if (teleport)
        setPosition(targetPosition);
}

void SceneNode::setTargetScale(sf::Vector2f scale) {
    targetScale = scale;
}

void SceneNode::setTargetScale(float pX, float pY) {
    targetScale = sf::Vector2f(pX, pY);
    // std::cerr << "Target scale set: " << targetScale.x << '\n';
}

void SceneNode::moveToWorldPosition(bool teleport) {
    setTargetPosition(getWorldPosition(), teleport);
}

sf::Vector2f SceneNode::getWorldPosition() const {
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->mParent) {
        transform = node->getTransform() * transform;
    }

    return transform;
}