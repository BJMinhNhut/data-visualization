#include <GUI/Console.hpp>
#include <Graphics/ResourceHolder.hpp>

namespace GUI {
Console::Console(const FontHolder& fonts)
    : Label(Mono, "", fonts), currentType(Info) {}

void Console::log(LogType type, const std::string& text) {
    if (type == Console::Info)
        setText("[INFO] " + text);
    else if (type == Console::Error)
        setText("[ERROR] " + text);

    currentType = type;
    setColor(getColorFromType(type));
}

void Console::clean() {
    setText("");
}

Console::LogType Console::getLogType() const {
    return currentType;
}

sf::Color Console::getColorFromType(LogType type) const {
    switch (type) {
        case Info:
            return sf::Color(13, 71, 161);
        case Error:
            return sf::Color(183, 28, 28);
        default:
            return sf::Color(13, 71, 161);
    }
}

}  // namespace GUI