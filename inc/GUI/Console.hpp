#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <GUI/Label.hpp>
#include <Graphics/ResourceHolder.hpp>
#include <Graphics/ResourceIdentifiers.hpp>

namespace GUI {

class Console : public Label {
   public:
    enum LogType {
        Info,
        Error,
    };

    typedef std::shared_ptr<Console> Ptr;

   public:
    Console(const FontHolder& fonts);
    void log(LogType type, const std::string& text);
    void clean();

    LogType getLogType() const;

   private:
    LogType currentType;
    sf::Color getColorFromType(LogType type) const;
};

}  // namespace GUI

#endif  // CONSOLE_HPP