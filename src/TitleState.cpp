#include <ResourceHolder.hpp>
#include <TitleState.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context),
      mText(),
      mShowText(),
      mTextEffectTime(sf::Time::Zero) {
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setFillColor(sf::Color::Black);

    mText.setString("Press any key to start");
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}

void TitleState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);

    if (mShowText)
        window.draw(mText);
}

bool TitleState::update(sf::Time dt) {
    mTextEffectTime += dt;

    if (mTextEffectTime >= sf::seconds(0.5f)) {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased) {
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}

bool TitleState::handleRealtime(const sf::Vector2i mousePosition) {
    return false;
}
