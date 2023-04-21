#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

// Forward declaration of SFML classes
namespace sf {
class Texture;
class Font;
}  // namespace sf

namespace Textures {
enum ID {
    TitleScreen,

    SinglyNodeNormal,

    HomeNormal,
    HomeSelected,

    CommandNormal,
    CommandSelected,
    CommandActivated,

    PlayNormal,
    PlaySelected,

    InputNormal,
    InputSelected,

    SmallButtonNormal,
    SmallButtonSelected,

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