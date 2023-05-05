#ifndef STATICARRAYCODE_HPP
#define STATICARRAYCODE_HPP

#include <string>

namespace StaticArrayCode {
const std::string insertMiddle = {
    "n++\n"
    "for(i = n-1; i > index; --i)\n"
    "	arr[i] = arr[i-1]\n"
    "arr[index] = value"};

const std::string insertBack = {
    "n++\n"
    "arr[n-1] = value"};

const std::string insertFront = {
    "n++\n"
    "for(i = n-1; i > 0; ++i)\n"
    "	arr[i] = arr[i-1]\n"
    "arr[0] = value"};

const std::string eraseMiddle = {
    "for(i = index; i+1 < n; ++i)\n"
    "	arr[i] = arr[i+1]\n"
    "n--"};

const std::string eraseBack = {"n--\n"};

const std::string eraseFront = {
    "for(i = 0; i+1 < n; ++i)\n"
    "	arr[i] = arr[i+1]\n"
    "n--"};

const std::string update = {"arr[index] = value"};

const std::string access = {"return arr[index]"};

const std::string search = {
    "if empty, return NOT_FOUND\n"
    "index = 0\n"
    "while (arr[i] != value)\n"
    "   index++\n"
    "   if index == n, return NOT_FOUND\n"
    "return index"};
};  // namespace StaticArrayCode

#endif  // STATICARRAYCODE_HPP