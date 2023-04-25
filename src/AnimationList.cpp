#include <AnimationList.hpp>

AnimationList::AnimationList()
    : currentAnimation(0),
      mIsPlaying(false),
      mCooldown(sf::seconds(0.f)),
      mSpeed(1.f),
      mList() {}

bool AnimationList::isFinished() const {
    return mList.size() > 0 && currentAnimation + 1 == mList.size();
}

unsigned int AnimationList::getProgress() const {
    return currentAnimation + 1;
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
    mIsPlaying = true;
}

void AnimationList::pause() {
    mIsPlaying = false;
}

void AnimationList::clear() {
    std::vector<Animation>().swap(mList);
    currentAnimation = 0;
    mIsPlaying = false;
    mCooldown = sf::seconds(0.f);
}

void AnimationList::playNext() {
    if (currentAnimation + 1 == mList.size())
        return;
    currentAnimation++;
    mList[currentAnimation].play();
    resetCooldown();
}

void AnimationList::playPrevious() {
    if (currentAnimation == 0)
        return;
    mList[currentAnimation].reverse();
    currentAnimation--;
    mList[currentAnimation].play();
    resetCooldown();
}

void AnimationList::goToFront() {
    while (currentAnimation > 0) {
        playPrevious();
    }
}

void AnimationList::goToBack() {
    while (currentAnimation + 1 < mList.size()) {
        playNext();
    }
}

void AnimationList::resetCooldown() {
    mCooldown = sf::seconds(0.f);
}

void AnimationList::setSpeed(const float& speed) {
    mSpeed = speed;
}

void AnimationList::update(sf::Time dt) {
    if (isPlaying()) {
        if (mCooldown < dt)
            mList[currentAnimation].play();

        if (currentAnimation + 1 == mList.size())
            pause();
        else {
            mCooldown += dt;
            if (mCooldown > sf::seconds(mSpeed * 1.f)) {
                currentAnimation++;
                resetCooldown();
            }
        }
    }
}