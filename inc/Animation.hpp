#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include <functional>

class Animation {
   public:
    Animation(const std::function<void()>& callback);

    void play();
    void reverse();

   private:
    std::function<void()> mCallback;
};

#endif  // ANIMATION_HPP