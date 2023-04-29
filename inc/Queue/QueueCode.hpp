#ifndef QUEUECODE_HPP
#define QUEUECODE_HPP

#include <Stack/StackCode.hpp>

namespace QueueCode {
const std::string enqueue{
    "Node myNode = new Node(value)\n"
    "if empty,\n"
    "   head = myNode\n"
    "else tail.next = myNode\n"
    "tail = myNode"};

const std::string dequeue = {
    "if empty, return\n"
    "Node myNode = head\n"
    "if head == tail,\n"
    "	head = tail = null\n"
    "else head = head.next\n"
    "delete myNode"};

const std::string clear = {
    "while (not empty)\n"
    "   dequeue"};
}  // namespace QueueCode

#endif