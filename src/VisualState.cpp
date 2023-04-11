#include <ResourceHolder.hpp>
#include <Utility.hpp>
#include <VisualState.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VisualState::VisualState(StateStack& stack, Context context)
    : State(stack, context), mScreen(*context.window) {
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
}

void VisualState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    mScreen.draw();
}

bool VisualState::update(sf::Time dt) {
    mScreen.update(dt);
    return true;
}

bool VisualState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) {
            // std::cerr << "Enter hit!\n";
            requestStackPop();
            requestStackPush(States::Visual);
        } else if (event.key.code == sf::Keyboard::Escape) {
            requestStackPop();
        } else if (event.key.code == sf::Keyboard::I) {
            mScreen.insertBack();
        } else if (event.key.code == sf::Keyboard::D) {
            mScreen.deleteBack();
        }
    }

    return true;
}

bool VisualState::handleRealtime(const sf::Vector2i mousePosition) {
    return false;
}
