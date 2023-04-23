#include <Animation.hpp>

Animation::Animation(const std::function<void()>& callback)
    : mCallback(callback) {}

void Animation::play() {
    mCallback();
}

void Animation::reverse() {}
