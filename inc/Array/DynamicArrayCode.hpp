#ifndef DYNAMICARRAYCODE_HPP
#define DYNAMICARRAYCODE_HPP

#include <string>

namespace DynamicArrayCode {
const std::string insertMiddle = {
    "new_arr = new int[n+1]\n"
    "for(i = 0; i < index; ++i)\n"
    "   new_arr[i] = arr[i]\n"
    "for(i = index; i < n; ++i)\n"
    "   new_arr[i+1] = arr[i]\n"
    "new_arr[index] = value\n"
    "delete arr\n"
    "arr = new_arr, n++\n"};

const std::string insertBack = {
    "new_arr = new int[n+1]\n"
    "for(i = 0; i < n; ++i)\n"
    "   new_arr[i] = arr[i]\n"
    "new_arr[n] = value\n"
    "delete arr\n"
    "arr = new_arr, n++\n"};

const std::string insertFront = {
    "new_arr = new int[n+1]\n"
    "for(i = 0; i < n; ++i)\n"
    "   new_arr[i+1] = arr[i]\n"
    "new_arr[0] = value\n"
    "delete arr\n"
    "arr = new_arr, n++\n"};

const std::string eraseMiddle = {
    "new_arr = new int[n-1]\n"
    "for(i = 0; i < index; ++i)\n"
    "   new_arr[i] = arr[i]\n"
    "for(i = index+1; i < n; ++i)\n"
    "   new_arr[i-1] = arr[i]\n"
    "delete arr\n"
    "arr = new_arr, n--\n"};

const std::string eraseBack = {
    "new_arr = new int[n-1]\n"
    "for(i = 0; i < n-1; ++i)\n"
    "   new_arr[i] = arr[i]\n"
    "delete arr\n"
    "arr = new_arr, n--\n"};

const std::string eraseFront = {
    "new_arr = new int[n-1]\n"
    "for(i = 1; i < n; ++i)\n"
    "   new_arr[i-1] = arr[i]\n"
    "delete arr\n"
    "arr = new_arr, n--\n"};

const std::string update = {"arr[index] = value"};

const std::string access = {"return arr[index]"};

const std::string search = {
    "if empty, return NOT_FOUND\n"
    "index = 0\n"
    "while (arr[i] != value)\n"
    "   index++\n"
    "   if index == n, return NOT_FOUND\n"
    "return index"};
};  // namespace DynamicArrayCode

#endif  // DYNAMICARRAYCODE_HPP