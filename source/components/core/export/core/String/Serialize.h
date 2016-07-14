#pragma once

#include <sstream>
#include <limits>
#include <ios>
#include <iostream>
#include <iomanip>

namespace Core
{
namespace String
{

template<typename T>
static std::string Serialize(const T value)
{
    static_assert(std::is_arithmetic<T>::value, 
                  "Arithmetic type required, please create partial specialization.");

    std::stringstream output;
    output << std::setprecision(std::numeric_limits<T>::max_digits10);
    output << value;
    return output.str();
}

template<>
inline std::string Serialize<std::string>(const std::string value)
{
    return value;
}

template<>
inline std::string Serialize<bool>(const bool value)
{
    std::stringstream output;
    output << std::boolalpha;
    output << value;
    return output.str();
}

} // namespace String
} // namespace Core
