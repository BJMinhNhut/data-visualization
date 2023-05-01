#include <Constants.hpp>
#include <GUI/Console.hpp>
#include <Graphics/ColorHolder.hpp>
#include <Graphics/ResourceHolder.hpp>
namespace GUI {
Console::Console(const FontHolder& fonts, const ColorHolder& colors)
    : Label(Mono, "", fonts, colors),
      currentType(Info),
      InfoColor(colors.get(Colors::Blue)),
      ErrorColor(colors.get(Colors::Red)) {}

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
            return InfoColor;
        case Error:
            return ErrorColor;
        default:
            return InfoColor;
    }
}

}  // namespace GUI