#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include <functional>

class Animation {
   public:
    Animation(const std::function<void()>& forward);
    Animation(const std::function<void()>& forward,
              const std::function<void()>& backward);

    void play();
    void reverse();

   private:
    std::function<void()> mForward;
    std::function<void()> mBackward;
};

#endif  // ANIMATION_HPP