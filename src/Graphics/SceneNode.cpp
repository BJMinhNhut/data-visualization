#include <Graphics/SceneNode.hpp>

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
    auto found =
        std::find_if(mChildren.begin(), mChildren.end(),
                     [&](Ptr& p) { return p.get() == node; });
    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::detachAllChildren() {
    std::vector<SceneNode*> detachList;
    for (Ptr& child : mChildren)
        detachList.push_back(child.get());

    for (SceneNode* ptr : detachList)
        detachChild(ptr);
}

void SceneNode::update(sf::Time dt) {
    // std::cerr << std::setprecision(3) << std::fixed << '\n';
    sf::Vector2f deltaPosition =
        (targetPosition - getPosition()) * 0.1f;
    move(deltaPosition);
    // std::cerr << targetPosition.x << ' ' << getPosition().x << '\n';

    sf::Vector2f deltaScale = (targetScale - getScale()) * 0.1f;
    // std::cerr << std::setprecision(3) << std::fixed << '\n';
    // std::cerr << targetScale.x << ' ' << getScale().x << ' ' << deltaScale.x
    //           << '\n';
    setScale(getScale() + deltaScale);

    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt) {
    // Do nothing as default
}

void SceneNode::updateChildren(sf::Time dt) {
    for (Ptr& child : mChildren) {
        child->update(dt);
    }
}

void SceneNode::draw(sf::RenderTarget& target,
                     sf::RenderStates states) const {
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

void SceneNode::setTargetPosition(sf::Vector2f position,
                                  Transition transition) {
    targetPosition = position;
    if (transition == None)
        setPosition(targetPosition);
}

void SceneNode::setTargetPosition(float pX, float pY,
                                  Transition transition) {
    setTargetPosition(sf::Vector2f(pX, pY), transition);
}

void SceneNode::setTargetScale(sf::Vector2f scale,
                               Transition transition) {
    targetScale = scale;
    if (transition == None)
        setScale(targetScale);
}

void SceneNode::setTargetScale(float pX, float pY,
                               Transition transition) {
    targetScale = sf::Vector2f(pX, pY);
    if (transition == None)
        setScale(targetScale);
    // std::cerr << "Target scale set: " << targetScale.x << '\n';
}

void SceneNode::moveToWorldPosition(Transition transition) {
    setTargetPosition(getWorldPosition(), transition);
}

sf::Vector2f SceneNode::getWorldPosition() const {
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr;
         node = node->mParent) {
        transform = node->getTransform() * transform;
    }

    return transform;
}

sf::Vector2f SceneNode::getTargetPosition() const {
    return targetPosition;
}

sf::Vector2f SceneNode::getLeftBound() const {
    return getWorldPosition();
}

sf::Vector2f SceneNode::getRightBound() const {
    return getWorldPosition();
}

sf::Vector2f SceneNode::getBottomBound() const {
    return getWorldPosition();
}

sf::Vector2f SceneNode::getTopBound() const {
    return getWorldPosition();
}