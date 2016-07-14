#include "osal/console.h"

#if defined(__GNUC__)

using namespace OSAL;

std::ostream * DetermineStream(int handle)
{
    switch (handle)
    {
    case STDOUT_FILENO:
        return &std::cout;
        break;
    case STDERR_FILENO:
        return &std::cerr;
        break;
    case STDIN_FILENO:
    default:
        std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

int DetermineHandle(std::ostream * stream)
{
	if (stream == &std::cout)
		return OSAL::fileno(stdout);
	else if (stream == &std::cerr)
		return OSAL::fileno(stderr);
	return -1;
}

const char * GetAnsiColorCode(ConsoleColor color)
{
	switch (color & ConsoleColor::ColorMask)
	{
	case ConsoleColor::Black:
		return "0";
	case ConsoleColor::Red:
		return "1";
	case ConsoleColor::Green:
		return "2";
	case ConsoleColor::Yellow:
		return "3";
	case ConsoleColor::Blue:
		return "4";
	case ConsoleColor::Magenta:
		return "5";
	case ConsoleColor::Cyan:
		return "6";
	case ConsoleColor::White:
		return "7";
	default:
		return nullptr;
	};
}

Console::Console(int handle)
	: stream(DetermineStream(handle))
	, handle(handle)
	, currentForegroundColor(ConsoleColor::Default)
	, currentBackgroundColor(ConsoleColor::Default)
{
}

Console::Console(std::ostream & stream)
	: stream(&stream)
	, handle(DetermineHandle(&stream))
	, currentForegroundColor(ConsoleColor::Default)
	, currentBackgroundColor(ConsoleColor::Default)
{
}

void Console::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, currentBackgroundColor);
}

void Console::SetBackgroundColor(ConsoleColor backgroundColor)
{
    SetTerminalColor(currentForegroundColor, backgroundColor);
}

void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (stream)
        *stream << command;
    currentForegroundColor = foregroundColor;
    currentBackgroundColor = backgroundColor;
}

void Console::ResetTerminalColor()
{
	SetTerminalColor();
}

bool Console::ShouldUseColor()
{
    if (handle == InvalidHandle)
        return false;
    if (!OSAL::isatty(handle))
        return false;

    const char * termSetting = getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
        (term == "xterm") ||
        (term == "xterm-color") ||
        (term == "xterm-256color") ||
        (term == "screen") ||
        (term == "screen-256color") ||
        (term == "linux") ||
        (term == "cygwin");
    return term_supports_color;
}

#endif // defined(__GNUC__)
