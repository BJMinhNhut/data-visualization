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
    void setTargetPosition(sf::Vector2f position, Transition transition);
    void setTargetPosition(float pX, float pY, Transition transition);
    void setTargetScale(sf::Vector2f scale, Transition transition);
    void setTargetScale(float pX, float pY, Transition transition);

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

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
    // TODO: node label
};

#endif  // SCENENODE_HPP