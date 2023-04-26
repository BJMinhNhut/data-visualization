#include <AnimationList.hpp>

#include <iostream>

AnimationList::AnimationList()
    : currentAnimation(0),
      mIsPlaying(false),
      mCooldown(sf::seconds(0.f)),
      mSpeed(1.f),
      mList() {}

bool AnimationList::isFinished() const {
    return mList.size() > 0 && currentAnimation == mList.size();
}

unsigned int AnimationList::getProgress() const {
    return currentAnimation;
}

unsigned int AnimationList::getSize() const {
    return mList.size();
}

float AnimationList::getSpeed() const {
    return mSpeed;
}

bool AnimationList::isEmpty() const {
    return mList.empty();
}

bool AnimationList::isPlaying() const {
    return mIsPlaying;
}

void AnimationList::push(const Animation& animation) {
    mList.push_back(animation);
}

void AnimationList::play() {
    std::cerr << "playing animation\n";
    mIsPlaying = true;
}

void AnimationList::pause() {
    mIsPlaying = false;
}

void AnimationList::clear() {
    std::cerr << "clear call\n";
    std::vector<Animation>().swap(mList);
    currentAnimation = 0;
    mIsPlaying = false;
    mCooldown = sf::seconds(0.f);
}

void AnimationList::playNext() {
    if (currentAnimation == mList.size())
        return;
    std::cerr << "step: " << currentAnimation << '\n';
    mList[currentAnimation].play();
    currentAnimation++;
    resetCooldown();
}

void AnimationList::playPrevious() {
    if (currentAnimation == 0)
        return;
    currentAnimation--;
    mList[currentAnimation].reverse();

    if (currentAnimation >= 1) {
        mList[currentAnimation - 1].reverse();
        mList[currentAnimation - 1].play();
    }
    resetCooldown();
}

void AnimationList::goToFront() {
    while (currentAnimation > 0)
        playPrevious();
    resetCooldown();
}

void AnimationList::goToBack() {
    while (currentAnimation < mList.size())
        playNext();
    resetCooldown();
}

void AnimationList::resetCooldown() {
    mCooldown = sf::seconds(0.f);
}

void AnimationList::setSpeed(const float& speed) {
    mSpeed = speed;
}

void AnimationList::update(sf::Time dt) {
    if (isPlaying()) {
        if (currentAnimation == mList.size())
            pause();
        else {
            mCooldown += dt;
            if (mCooldown > sf::seconds(1.f / mSpeed))
                playNext();
        }
    }
}