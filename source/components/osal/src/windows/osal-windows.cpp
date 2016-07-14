#include <osal.h>

#if defined(_MSC_VER)

#include <fcntl.h>
#include <sys/stat.h>

#pragma warning(push)
#pragma warning(disable:4091)
#include <Dbghelp.h>
#pragma warning(pop)

using namespace OSAL;

static char * P_tmpdir;

class Initializer
{
public:
	Initializer()
	{
		GetTempPathA(_MAX_PATH, TempDir);
		P_tmpdir = TempDir;
	}
private:
	char TempDir[_MAX_PATH];
};

static Initializer initializer;

std::string OSAL::ConvertTypeName(std::string mangledName)
{
	std::string result;
	SymSetOptions(SYMOPT_UNDNAME);
	char demangledName[_MAX_PATH];
	size_t size = UnDecorateSymbolName(mangledName.c_str() + 1, demangledName, _MAX_PATH, UNDNAME_32_BIT_DECODE | UNDNAME_NAME_ONLY);
	if (size > 0)
	{
		result = demangledName;
	}
	return result;
}

static LARGE_INTEGER GetFileTimeOffset()
{
	SYSTEMTIME s;
	FILETIME f;
	LARGE_INTEGER t;

	s.wYear = 1970;
	s.wMonth = 1;
	s.wDay = 1;
	s.wHour = 0;
	s.wMinute = 0;
	s.wSecond = 0;
	s.wMilliseconds = 0;
	SystemTimeToFileTime(&s, &f);
	t.QuadPart = f.dwHighDateTime;
	t.QuadPart <<= 32;
	t.QuadPart |= f.dwLowDateTime;
	return (t);
}

size_t OSAL::getline(char ** lineptr, size_t * n, FILE * stream)
{
	char *bufptr = nullptr;
	char *p = bufptr;
	size_t size;
	int c;

	if (lineptr == nullptr)
		return -1;
	if (stream == nullptr)
		return -1;
	if (n == nullptr)
		return -1;
	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF) {
		return -1;
	}
	if (bufptr == nullptr)
	{
		bufptr = (char *)malloc(128);
		if (bufptr == nullptr)
			return -1;
		size = 128;
	}
	p = bufptr;
	while (c != EOF)
	{
		if ((p - bufptr) > ((ssize_t)size - 1))
		{
			size = size + 128;
			bufptr = (char *)realloc(bufptr, size);
			if (bufptr == nullptr)
				return -1;
		}
		*p++ = c;
		if (c == '\n')
			break;
		c = fgetc(stream);
	}

	*p++ = '\0';
	*lineptr = bufptr;
	*n = size;

	return p - bufptr - 1;
}

int OSAL::clock_getres(int X, struct timespec * tv)
{
    if (!tv)
        return EFAULT;
    if (X != CLOCK_REALTIME)
        return EINVAL;
    LARGE_INTEGER performanceFrequency;
    BOOL result = QueryPerformanceFrequency(&performanceFrequency);
    tv->tv_sec = long(performanceFrequency.QuadPart / 1000000000);
    tv->tv_nsec = long(performanceFrequency.QuadPart % 1000000000);
    return result ? 0 : EINVAL;
}

    int OSAL::clock_gettime(int X, struct timespec * tv)
{
	LARGE_INTEGER t;
	FILETIME f;
	double                  microseconds;
	static LARGE_INTEGER    offset {};
	static double           frequencyToMicroseconds;
	static bool             initialized {};
	static BOOL             usePerformanceCounter {};

    if (!tv)
        return EFAULT;
    if (X != CLOCK_REALTIME)
        return EINVAL;
    if (!initialized)
	{
		LARGE_INTEGER performanceFrequency;
		initialized = true;
		usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
		if (usePerformanceCounter)
		{
			QueryPerformanceCounter(&offset);
			frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.0;
		}
		else
		{
			offset = GetFileTimeOffset();
			frequencyToMicroseconds = 10.;
		}
	}
	if (usePerformanceCounter)
		QueryPerformanceCounter(&t);
	else
	{
		GetSystemTimeAsFileTime(&f);
		t.QuadPart = f.dwHighDateTime;
		t.QuadPart <<= 32;
		t.QuadPart |= f.dwLowDateTime;
	}

	t.QuadPart -= offset.QuadPart;
	microseconds = (double)t.QuadPart / frequencyToMicroseconds;
	t.QuadPart = LONGLONG(microseconds);
	tv->tv_sec = long(t.QuadPart / 1000000000);
	tv->tv_nsec = long(t.QuadPart % 1000000000);
	time_t timeSeconds;
	timeSeconds = time(nullptr);
	tv->tv_sec += long(timeSeconds);
	return (0);
}

#pragma warning (disable: 4996)
char const * OSAL::tzname(tm const & UNUSED(dateTime)) { return *::__tzname(); }
long int OSAL::get_timezone(tm const & UNUSED(dateTime)) { return - *::__timezone(); }
#pragma warning (default: 4996)

/* FILETIME of Jan 1 1970 00:00:00. */
static const unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);

/*
* timezone information is stored outside the kernel so tzp isn't used anymore.
*
* Note: this function is not for Win32 high precision timing purpose. See
* elapsed_time().
*/
int OSAL::gettimeofday(struct timeval * time, struct timezone * timeZone)
{
	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.u.LowPart = file_time.dwLowDateTime;
	ularge.u.HighPart = file_time.dwHighDateTime;

	time->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
	time->tv_usec = (long)(system_time.wMilliseconds * 1000);

	return 0;
}

void OSAL::usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

int OSAL::nanosleep(const struct timespec *req, struct timespec *rem)
{
	HANDLE hTimer = CreateWaitableTimer(nullptr, false, nullptr);
	LARGE_INTEGER dueTime;
	LONGLONG parts = req->tv_nsec / 100 + req->tv_sec * 10000000LL;
	dueTime.QuadPart = -parts;

	if (!SetWaitableTimer(hTimer, &dueTime, 0, nullptr, nullptr, 0))
		return EINVAL;

	// Wait for the timer.
	if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		return EINTR;
	return 0;
}

char * OSAL::Tmpdir()
{
     return ::P_tmpdir;
}

int OSAL::mkstemp(char * fileNameTemplate)
{
	char tempFileName[_MAX_PATH];
	std::string path = fileNameTemplate;
	std::string fileName = path;

	size_t lastPathDelimiterPos = path.find_last_of('\\');
	if (lastPathDelimiterPos != std::string::npos)
	{
		fileName = path.substr(lastPathDelimiterPos + 1);
	}

	if (GetTempFileNameA(P_tmpdir, fileName.c_str(), 0, tempFileName) == 0)
		return -1;
	strcpy(fileNameTemplate, tempFileName);
	return open(tempFileName, _O_CREAT | _O_TEMPORARY, _S_IREAD | _S_IWRITE);
}

int OSAL::strcasecmp(char const * s1, char const * s2)
{
	return _stricmp(s1, s2);
}

int OSAL::wcscasecmp(wchar_t const * s1, wchar_t const * s2)
{
	while (*s1 != '\0' && towlower(*s1) == towlower(*s2))
	{
		s1++;
		s2++;
	}

	return towlower(*s1) - towlower(*s2);
}

//int Platform::stat(char const * path, StatStruct & buf)
//{
//	return ::_stat(path, &buf);
//}
//char * Platform::strdup(char const * src)
//{
//	return ::_strdup(src);
//}
//int Platform::rmdir(char const * dir)
//{
//	return ::_rmdir(dir);
//}
//bool Platform::isdir(StatStruct const & st)
//{
//	return S_ISDIR(st.st_mode);
//}
//
//char const * Platform::strncpy(char * dest, char const * src, size_t n)
//{
//	return ::strncpy(dest, src, n);
//}
//
//int Platform::chdir(char const * dir)
//{
//	return ::_chdir(dir);
//}
//FILE * Platform::freopen(char const * path, char const * mode, FILE * stream)
//{
//	return ::freopen(path, mode, stream);
//}
//FILE * Platform::fopen(char const * path, char const * mode)
//{
//	return ::fopen(path, mode);
//}
//
//int Platform::read(int fd, void * buf, unsigned int count)
//{
//	return static_cast<int>(::_read(fd, buf, count));
//}
//int Platform::write(int fd, void const * buf, unsigned int count)
//{
//	return static_cast<int>(::_write(fd, buf, count));
//}
//int Platform::close(int fd)
//{
//	return ::_close(fd);
//}
//
//char const * Platform::getenv(char const * name)
//{
//	return ::getenv(name);
//}
//
//void Platform::abort()
//{
//	::abort();
//}
//
#endif // defined(_MSC_VER)
