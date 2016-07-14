#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#undef min
#undef max
#include <string>
#include <ctime>
//#include <sys/stat.h>
#include <conio.h>
#include <direct.h>
#include <io.h>
//#include <time.h>
//#include <string.h>
//#include <cstdio>
//
#define bswap_16(x) ((x) >> 8 | ((x)& 255) << 8)
#define bswap_32(x) ((bswap_16((x) >> 16) & 0xFFFF) | (bswap_16((x) & 0xFFFF) << 16))
#define bswap_64(x) ((bswap_32((x) >> 32) & 0xFFFFFFFF) | (bswap_32((x) & 0xFFFFFFFF) << 32))

typedef SSIZE_T ssize_t;

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#define	__S_IREAD	0400	// Read by owner.
#define	__S_IWRITE	0200	// Write by owner.
#define	__S_IEXEC	0100	// Execute by owner.

#define	S_IRUSR	__S_IREAD	// Read by owner.
#define	S_IWUSR	__S_IWRITE	// Write by owner.
#define	S_IXUSR	__S_IEXEC	// Execute by owner.
// Read, write, and execute by owner.
#define	S_IRWXU	(__S_IREAD|__S_IWRITE|__S_IEXEC)

#define	S_IRGRP	(S_IRUSR >> 3)	// Read by group.
#define	S_IWGRP	(S_IWUSR >> 3)	// Write by group.
#define	S_IXGRP	(S_IXUSR >> 3)	// Execute by group.
// Read, write, and execute by group.
#define	S_IRWXG	(S_IRWXU >> 3)

#define	S_IROTH	(S_IRGRP >> 3)	// Read by others.
#define	S_IWOTH	(S_IWGRP >> 3)	// Write by others.
#define	S_IXOTH	(S_IXGRP >> 3)	// Execute by others.
// Read, write, and execute by others.
#define	S_IRWXO	(S_IRWXG >> 3)

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

//// The testing::internal::posix namespace holds wrappers for common
//// POSIX functions.  These wrappers hide the differences between
//// Windows/MSVC and POSIX systems.  Since some compilers define these
//// standard functions as macros, the wrapper cannot have the same name
//// as the wrapped function.

namespace OSAL
{

#pragma warning (disable: 4996)
// Console
inline int kbhit(void)
{
	return ::_kbhit();
}

// Error information// Error information

inline char const * strerror(int errnum)
{
    return ::strerror(errnum);
}

// Path handling

inline int unlink(const char * pathname)
{
	return ::_unlink(pathname);
}

inline int chdir(char const * dir)
{
	return _chdir(dir);
}

inline int mkdir(const char * pathname, int mode)
{
	return ::_mkdir(pathname);
}

inline char * realpath(const char *path, char *resolved_path)
{
	return _fullpath(resolved_path, path, _MAX_PATH);
}

inline char * get_current_dir_name()
{
	char * buffer = (char *)malloc(_MAX_PATH);
	if (GetCurrentDirectoryA(_MAX_PATH, buffer) == 0)
	{
		return nullptr;
	}
	return buffer;
}

inline int open(const char * path, int flags, int mode)
{
	return ::_open(path, flags, mode);
}

inline FILE * fdopen(int fd, char const * mode)
{
	return ::_fdopen(fd, mode);
}

inline int fclose(FILE * fp)
{
	return ::fclose(fp);
}

inline int fileno(FILE * file)
{
	return ::_fileno(file);
}

inline int isatty(int fd)
{
	return ::_isatty(fd);
}


// File handling

FILE * fdopen(int fd, char const * mode);
int fclose(FILE * fp);
int fileno(FILE * file);
int isatty(int fd);

// File read/write

size_t getline(char **lineptr, size_t *n, FILE *stream);

// Time / date
/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME       0
/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC      1
/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID 2
/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID  3
/* Monotonic system-wide clock, not adjusted for frequency scaling.  */
#define CLOCK_MONOTONIC_RAW      4
/* Identifier for system-wide realtime clock, updated only on ticks.  */
#define CLOCK_REALTIME_COARSE    5
/* Monotonic system-wide clock, updated only on ticks.  */
#define CLOCK_MONOTONIC_COARSE   6

int clock_getres(int X, struct timespec * tv);
int clock_gettime(int X, struct timespec * tv);
char const * tzname(tm const & dateTime);
long int get_timezone(tm const & dateTime);

inline tm make_tm(int second, int minute, int hour, int day, int month, int year)
{ return tm { second, minute, hour, day, month - 1, year - 1900, 0, 0, 0 }; }
int gettimeofday(struct timeval * time, struct timezone * timeZone);
void usleep(__int64 usec);
int nanosleep(const struct timespec *req, struct timespec *rem);

// Temp file handling

char * Tmpdir();
int mkstemp(char * fileNameTemplate);

// String handling

int strcasecmp(char const * s1, char const * s2);
int wcscasecmp(wchar_t const * s1, wchar_t const * s2);

// Environment handling

inline char const * getenv(char const * name)
{
#pragma warning (disable: 4996)
	return ::getenv(name);
#pragma warning (default: 4996)
}

//// Functions with a different name on Windows.
//
//typedef struct _stat StatStruct;
//
//int stat(char const * path, StatStruct & buf);
//char * strdup(char const * src);
//int rmdir(char const * dir);
//bool isdir(StatStruct const & st);
//char const * strncpy(char * dest, char const * src, size_t n);
//FILE * fopen(char const * path, char const * mode);
//FILE * freopen(char const * path, char const * mode, FILE * stream);
//int open(const char * path, int flags, int mode);
//int read(int fd, void * buf, unsigned int count);
//int write(int fd, void const * buf, unsigned int count);
//int close(int fd);
//void abort();
//

#pragma warning (default: 4996)

}  // namespace OSAL

