#ifndef SLLCODE_HPP
#define SLLCODE_HPP

#include <string>

namespace SLLCode {
const std::string insertFront = {
    "Node myNode = new Node(value)\n"
    "myNode.next = head\n"
    "head = myNode"};

const std::string insertMiddle = {
    "Node cur = head\n"
    "for(k = 0; k+1 < index; ++k)\n"
    "	cur = cur.next\n"
    "Node myNode = new Node(value)\n"
    "myNode.next = cur.next\n"
    "cur.next = myNode"};

const std::string eraseFront = {
    "Node myNode = head\n"
    "head = head.next\n"
    "delete myNode"};

const std::string eraseMiddle = {
    "Node cur = head\n"
    "for(k = 0; k+1 < index; ++k)\n"
    "	cur = cur.next\n"
    "Node myNode = cur.next\n"
    "cur.next = myNode.next\n"
    "delete myNode"};

const std::string update = {
    "Node cur = head\n"
    "for(k = 0; k < index; ++k)\n"
    "	cur = cur.next\n"
    "cur.data = value"};

const std::string search = {
    "if empty, return NOT_FOUND\n"
    "Node cur = head\n"
    "int index = 0\n"
    "while (cur.data != value)\n"
    "   index++, cur = cur.next\n"
    "   if cur == null,\n"
    "       return NOT_FOUND\n"
    "return index"};

}  // namespace SLLCode

#endif  // SLLCODE_HPP