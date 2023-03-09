#include "main.hpp"

Game::Game():
mWindow(sf::VideoMode(640, 480), "WTF???"),
mPlayer(), mTexture(),
mIsMovingDown(false),
mIsMovingUp(false),
mIsMovingLeft(false),
mIsMovingRight(false) {
    if (!mTexture.loadFromFile("data/images/texture2.jpg")) {}
    mPlayer.setTexture(mTexture);
    mPlayer.setPosition(100.f, 100.f);
    mWindow.setVerticalSyncEnabled(true);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}

void Game::update(sf::Time deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    float PlayerSpeed = 1.f/deltaTime.asSeconds();
    if (mIsMovingUp) movement.y -= PlayerSpeed;
    if (mIsMovingDown) movement.y += PlayerSpeed;
    if (mIsMovingLeft) movement.x -= PlayerSpeed;
    if (mIsMovingRight) movement.x += PlayerSpeed;
    mPlayer.move(movement * deltaTime.asSeconds());
}

void Game::render() {
    auto [x, y] = mPlayer.getPosition();
    // std::cerr << x << ' ' << y << '\n';
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::W) 
        mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;  
    else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
}

int main()
{
    Game game;
    game.run();
    return 0;
}