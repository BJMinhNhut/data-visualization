#include <Animation.hpp>

Animation::Animation(const std::function<void()>& forward)
    : mBackward(), mForward(forward) {}

Animation::Animation(const std::function<void()>& forward,
                     const std::function<void()>& backward)
    : mBackward(backward), mForward(forward) {}

void Animation::play() {
    mForward();
}

void Animation::reverse() {
    mBackward();
}
