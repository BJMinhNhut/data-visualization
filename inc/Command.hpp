#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <SceneNode.hpp>

#include <functional>

struct Command {
	Command();

	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

#endif COMMAND_HPP