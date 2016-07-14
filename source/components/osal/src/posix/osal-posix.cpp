#include <osal.h>

#if defined(__GNUC__)

#include <cxxabi.h>
#include <string.h>
#include <termios.h>

using namespace OSAL;

std::string OSAL::ConvertTypeName(std::string mangledName)
{
	std::string result;
	int status;
	char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
	if (status == 0)
	{
		result = demangledName;
		std::free(demangledName);
	}
	return result;
}

int OSAL::kbhit()
{
	int character;
	struct termios orig_term_attr;
	struct termios new_term_attr;

	/* set the terminal to raw mode */
	tcgetattr(OSAL::fileno(stdin), &orig_term_attr);
	memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
	new_term_attr.c_lflag &= ~(ECHO | ICANON);
	new_term_attr.c_cc[VTIME] = 0;
	new_term_attr.c_cc[VMIN] = 0;
	tcsetattr(OSAL::fileno(stdin), TCSANOW, &new_term_attr);

	/* read a character from the stdin stream without blocking */
	/*   returns EOF (-1) if no character is available */
	character = fgetc(stdin);

	/* restore the original terminal attributes */
	tcsetattr(OSAL::fileno(stdin), TCSANOW, &orig_term_attr);

	return character;
}

#endif // defined(__GNUC__)
