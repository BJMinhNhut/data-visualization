#ifndef CLLCODE_HPP
#define CLLCODE_HPP

#include <string>

namespace CLLCode {
const std::string insertFront = {
    "Node myNode = new Node(value)\n"
    "if head == nullptr,\n"
    "   myNode.next = myNode\n"
    "   head = tail = myNode\n"
    "else\n"
    "   myNode.next = head\n"
    "   head = myNode\n"
    "   tail.next = myNode\n"};

const std::string insertBack = {
    "Node myNode = new Node(value)\n"
    "tail.next = myNode\n"
    "myNode.next = head\n"
    "tail = myNode"};

const std::string insertMiddle = {
    "Node cur = head\n"
    "for(k = 0; k+1 < index; ++k)\n"
    "	cur = cur.next\n"
    "Node myNode = new Node(value)\n"
    "myNode.next = cur.next\n"
    "cur.next = myNode"};

const std::string eraseFront = {
    "Node myNode = head\n"
    "if head.next == null,\n"
    "   head = tail = null\n"
    "else tail.next = head = head.next\n"
    "delete myNode"};

const std::string eraseBack = {
    "Node pre = head\n"
    "while (pre.next != tail)\n"
    "	pre = pre.next\n"
    "delete tail, tail = pre\n"
    "tail.next = head"};

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
    "   if cur == head,\n"
    "       return NOT_FOUND\n"
    "return index"};

}  // namespace CLLCode

#endif  // CLLCODE_HPP