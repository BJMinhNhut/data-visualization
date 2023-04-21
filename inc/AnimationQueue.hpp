#ifndef ANIMATIONQUEUE_HPP
#define ANIMATIONQUEUE_HPP

#include "Animation.hpp"

#include <vector>

class AnimationList {
   public:
    float getProgress();
    Animation getNext();
    Animation getPrev();
    void reset();
    bool isEmpty();
    bool isPaused();

   private:
    unsigned int currentAnimation;
    bool pause;
    std::vector<Animation> mList;
};

#endif  // ANIMATIONQUEUE_HPP