#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>

class SceneNode : public sf::Transformable,
                  public sf::Drawable,
                  public sf::NonCopyable {
   public:
    typedef std::unique_ptr<SceneNode> Ptr;
    enum Transition {
        None,
        Smooth,
    };

   public:
    SceneNode();

    void attachChild(Ptr child);
    Ptr detachChild(SceneNode* node);
    void detachAllChildren();

    void update(sf::Time dt);

    void moveToWorldPosition(Transition transition);
    virtual void setTargetPosition(sf::Vector2f position,
                                   Transition transition);
    virtual void setTargetPosition(float pX, float pY, Transition transition);
    virtual void setTargetScale(sf::Vector2f scale, Transition transition);
    virtual void setTargetScale(float pX, float pY, Transition transition);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;
    sf::Vector2f getTargetPosition() const;

    virtual sf::Vector2f getLeftBound() const;
    virtual sf::Vector2f getRightBound() const;
    virtual sf::Vector2f getBottomBound() const;

   private:
    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

   private:
    sf::Vector2f targetPosition;
    sf::Vector2f targetScale;
    std::vector<Ptr> mChildren;
    SceneNode* mParent;
};

#endif  // SCENENODE_HPP