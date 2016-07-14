#pragma once

#include <iostream>
#include "osal.h"
#include "osal/flagoperators.h"

namespace OSAL
{

    class Secret;

} // namespace OSAL

// Ensures that there is at least one operator<< in the global namespace.
// See Message& operator<<(...) below for why.
void operator<<(const OSAL::Secret&, int);

namespace OSAL
{

#if defined(__GNUC__)
enum class ConsoleColor : int
{
    Default = -1,
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    ColorMask = 0x07,
    Intensity = 0x08,
    Bold = 0x10,
};
#elif defined (_MSC_VER)
	enum class ConsoleColor : int
	{
		Default = -1,
		Black = 0,
		Red = FOREGROUND_RED,
		Green = FOREGROUND_GREEN,
		Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
		Blue = FOREGROUND_BLUE,
		Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
		Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
		White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		ColorMask = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		Intensity = FOREGROUND_INTENSITY,
		Bold = 0,
	};
#endif
DEFINE_FLAG_OPERATORS(ConsoleColor, int);

std::ostream & operator << (std::ostream & stream, ConsoleColor value);

class Console
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::ostream & (* BasicNarrowIoManip)(std::ostream &);

public:
    static const int InvalidHandle = -1;

    Console(int handle = OSAL::fileno(stdout));
    Console(std::ostream & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default, ConsoleColor backgroundColor = ConsoleColor::Default);
	void ResetTerminalColor();
	bool ShouldUseColor();

    // Streams a non-pointer value to this object.
    template <typename T>
    inline Console & operator << (const T & val)
    {
        using ::operator <<;
        if (stream)
            *stream << val;
        return *this;
    }

    Console & operator << (BasicNarrowIoManip val)
    {
        if (stream)
            *stream << val;
        return *this;
    }

protected:
    std::ostream * stream;
    int handle;
    ConsoleColor currentForegroundColor;
    ConsoleColor currentBackgroundColor;
#if defined(_MSC_VER)
	WORD defaultColorAttributes;
#endif

};

struct _SetForegroundColor
{
    ConsoleColor color;
};

struct _SetBackgroundColor
{
    ConsoleColor color;
};

inline Console & operator << (Console & stream, _SetForegroundColor color)
{
    stream.SetForegroundColor(color.color);
    return stream;
}

inline Console & operator << (Console & stream, _SetBackgroundColor color)
{
    stream.SetBackgroundColor(color.color);
    return stream;
}

} // namespace OSAL

inline OSAL::_SetForegroundColor fgcolor(OSAL::ConsoleColor color)
{
	return{ color };
}

inline OSAL::_SetBackgroundColor bgcolor(OSAL::ConsoleColor color)
{
	return{ color };
}

