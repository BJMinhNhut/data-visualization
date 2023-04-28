#ifndef STACKCODE_HPP
#define STACKCODE_HPP

#include <SLL/SLLCode.hpp>

namespace StackCode {
const std::string push = SLLCode::insertFront;
const std::string pop = {
    "if empty, return\n"
    "Node myNode = head\n"
    "head = head.next\n"
    "delete myNode"};
}  // namespace StackCode

#endif  // STACKCODE_HPP