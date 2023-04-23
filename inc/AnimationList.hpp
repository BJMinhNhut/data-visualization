#ifndef ANIMATIONLIST_HPP
#define ANIMATIONLIST_HPP

#include <Animation.hpp>

#include <SFML/System/NonCopyable.hpp>

#include <vector>

class AnimationList {
   public:
    AnimationList();

    unsigned int getProgress() const;
    unsigned int getSize() const;
    float getSpeed() const;
    bool isEmpty() const;
    bool isPlaying() const;

    void push(const Animation& animation);
    void clear();

    void play();
    void pause();
    void playNext();
    void playPrevious();
    void goToFront();
    void goToBack();

    void resetCooldown();
    void setSpeed(const float& speed);

    void update(sf::Time dt);

   private:
    unsigned int currentAnimation;
    sf::Time mCooldown;
    float mSpeed;
    bool mIsPlaying;
    std::vector<Animation> mList;
};

#endif  // ANIMATIONLIST_HPP