#include "osal/console.h"

#if defined(_MSC_VER)

using namespace OSAL;

static const uint16_t DefaultColorAttributes = 0x07;

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

Console::Console(int handle)
	: stream(DetermineStream(handle))
	, handle(handle)
	, currentForegroundColor(ConsoleColor::Default)
	, currentBackgroundColor(ConsoleColor::Default)
	, defaultColorAttributes()
{
	CONSOLE_SCREEN_BUFFER_INFO  consoleInfo;
	HANDLE streamHandle = (this->stream == &std::cout) ? GetStdHandle(STD_OUTPUT_HANDLE) : GetStdHandle(STD_ERROR_HANDLE);

	if (GetConsoleScreenBufferInfo(streamHandle, &consoleInfo))
		defaultColorAttributes = consoleInfo.wAttributes;
}

Console::Console(std::ostream & stream)
	: stream(&stream)
	, handle(DetermineHandle(&stream))
	, currentForegroundColor(ConsoleColor::Default)
	, currentBackgroundColor(ConsoleColor::Default)
	, defaultColorAttributes()
{
	CONSOLE_SCREEN_BUFFER_INFO  consoleInfo;
	HANDLE streamHandle = (this->stream == &std::cout) ? GetStdHandle(STD_OUTPUT_HANDLE) : GetStdHandle(STD_ERROR_HANDLE);

	if (GetConsoleScreenBufferInfo(streamHandle, &consoleInfo))
		defaultColorAttributes = consoleInfo.wAttributes;
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
	HANDLE handle = (stream == &std::cout) ? GetStdHandle(STD_OUTPUT_HANDLE) : GetStdHandle(STD_ERROR_HANDLE);

	int textColor = int(defaultColorAttributes) & 0x0F;
	if (foregroundColor != ConsoleColor::Default)
	{
		textColor = int(foregroundColor) & 0x0F;
	}
	int backColor = (int(defaultColorAttributes) >> 4) & 0x0F;
	if (backgroundColor != ConsoleColor::Default)
	{
		backColor = int(backgroundColor) & 0x0F;
	}
	unsigned short colorAttributes = ((unsigned)backColor << 4) | (unsigned)textColor;
	SetConsoleTextAttribute(handle, colorAttributes);
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

	return true;
}

#endif // defined(_MSC_VER)
