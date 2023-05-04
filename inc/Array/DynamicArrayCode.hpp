#ifndef DYNAMICARRAYCODE_HPP
#define DYNAMICARRAYCODE_HPP

#include <string>

namespace DynamicArrayCode {
const std::string insertMiddle = {
    "n++\n"
    "for(i = n-1; i > index; --i)\n"
    "	a[i] = a[i-1]\n"
    "a[index] = value"};

const std::string insertBack = {
    "n++\n"
    "a[n-1] = value"};

const std::string insertFront = {
    "n++\n"
    "for(i = n-1; i > 0; ++i)\n"
    "	a[i] = a[i-1]\n"
    "a[0] = value"};

const std::string eraseMiddle = {
    "for(i = index; i+1 < n; ++i)\n"
    "	a[i] = a[i+1]\n"
    "n--"};

const std::string eraseBack = {"n--\n"};

const std::string eraseFront = {
    "for(i = 0; i+1 < n; ++i)\n"
    "	a[i] = a[i+1]\n"
    "n--"};

const std::string update = {"a[index] = value"};

const std::string access = {"return a[index]"};

const std::string search = {
    "if empty, return NOT_FOUND\n"
    "index = 0\n"
    "while (a[i] != value)\n"
    "   index++\n"
    "   if index == n, return NOT_FOUND\n"
    "return index"};
};  // namespace DynamicArrayCode

#endif  // DYNAMICARRAYCODE_HPP