#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

// Forward declaration of SFML classes
namespace sf {
class Texture;
class Font;
}  // namespace sf

namespace Textures {
enum ID {
    Background,

    SinglyNode,

    CheckBoxNormal,
    CheckBoxSelected,
    CheckBoxActivated,

    HomeNormal,
    HomeSelected,
    BackNormal,
    BackSelected,
    TitleBar,

    CommandNormal,
    CommandSelected,
    CommandActivated,

    PlayNormal,
    PlaySelected,
    PauseNormal,
    PauseSelected,
    ReplayNormal,
    ReplaySelected,

    ArrowDownNormal,
    ArrowDownSelected,
    ArrowNormal,
    ArrowSelected,
    DoubleArrowNormal,
    DoubleArrowSelected,

    InputNormal,
    InputSelected,

    SmallButtonNormal,
    SmallButtonSelected,
    BigButtonNormal,
    BigButtonSelected,

    MenuSLLNormal,
    MenuSLLSelected,
    MenuDLLNormal,
    MenuDLLSelected,
    MenuCLLNormal,
    MenuCLLSelected,
    MenuStackNormal,
    MenuStackSelected,
    MenuQueueNormal,
    MenuQueueSelected,
    MenuStaticNormal,
    MenuStaticSelected,
    MenuDynamicNormal,
    MenuDynamicSelected,
};
}

namespace Fonts {
enum ID {
    Main,
    Mono,
    Medium,
    Bold,
};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif  //RESOURCEIDENTIFIERS_HPP