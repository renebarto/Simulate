#pragma once

// Standard operators for integer type derived enum class types.

#define DEFINE_FLAG_OPERATORS(T, B) \
inline T operator | (T const & lhs, T const & rhs) \
{ \
    return T(B(lhs) | B(rhs)); \
} \
inline T operator & (T const & lhs, T const & rhs) \
{ \
    return T(B(lhs) & B(rhs)); \
} \
inline T operator ^ (T const & lhs, T const & rhs) \
{ \
    return T(B(lhs) ^ B(rhs)); \
} \
inline T & operator |= (T & lhs, T const & rhs) \
{ \
    lhs = T(B(lhs) | B(rhs)); \
    return lhs; \
} \
inline T & operator &= (T & lhs, T const & rhs) \
{ \
    lhs = T(B(lhs) & B(rhs)); \
    return lhs; \
} \
inline T & operator ^= (T & lhs, T const & rhs) \
{ \
    lhs = T(B(lhs) ^ B(rhs)); \
    return lhs; \
} \
inline T operator ~ (T const & flags) \
{ \
    return T(~B(flags)); \
} \
inline bool operator ! (T const & flags) \
{ \
    return B(flags) == 0; \
} \
inline bool operator == (B const & lhs, T const & rhs) \
{ \
    return lhs == B(rhs); \
} \
inline bool operator != (B const & lhs, T const & rhs) \
{ \
    return lhs != B(rhs); \
} \
inline bool operator == (T const & lhs, B const & rhs) \
{ \
    return B(lhs) == rhs; \
} \
inline bool operator != (T const & lhs, B const & rhs) \
{ \
    return B(lhs) != rhs; \
}
