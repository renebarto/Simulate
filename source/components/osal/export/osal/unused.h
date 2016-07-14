#pragma once

#ifdef UNUSED
#elif defined(__GNUC__)
#define UNUSED(x) x __attribute__((unused))
#else
#define UNUSED(x) x
#endif
