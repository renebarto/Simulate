#pragma once

#include <sstream>
#include <limits>
#include <ios>
#include <iostream>
#include <iomanip>
#include <type_traits>

namespace Core {
namespace String {

template<typename T>
static T Deserialize(const std::string& value) {
    static_assert(std::is_arithmetic<T>::value, 
                  "Arithmetic type required, please create partial specialization.");

    T output;
    std::stringstream input(value);
    input >> output;
    return output;
}

template<>
std::string Deserialize<std::string>(const std::string& value) {
    return value;
}

template<>
bool Deserialize<bool>(const std::string &value) {
    bool output;
    std::stringstream input(value);
    input >> std::boolalpha >> output;
    return output;
}

} // namespace String
} // namespace Core
