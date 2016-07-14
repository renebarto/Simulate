#pragma once

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <byteswap.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>

#ifndef MAX_PATH
const int MAX_PATH = 4096;
#endif

namespace OSAL
{

// Console

int kbhit();

// Error information

inline char const * strerror(int errnum)
{
    return ::strerror(errnum);
}

// Path handling

inline int unlink(const char * pathname)
{
    return ::unlink(pathname);
}

inline int chdir(char const * dir)
{
	return ::chdir(dir);
}

inline int mkdir(const char * pathname, int mode)
{
    return ::mkdir(pathname, mode);
}

inline char * realpath(const char * path, char * resolved_path)
{
    return ::realpath(path, resolved_path);
}

inline char * get_current_dir_name(void)
{
     return ::get_current_dir_name();
}

// File handling

inline FILE * fdopen(int fd, char const * mode)
{
    return ::fdopen(fd, mode);
}
inline int fclose(FILE * fp)
{
    return ::fclose(fp);
}

inline int fileno(FILE * file)
{
    return ::fileno(file);
}

inline int isatty(int fd)
{
    return ::isatty(fd);
}

// File read/write

inline size_t getline(char **lineptr, size_t *n, FILE *stream)
{
    return ::getline(lineptr, n, stream);
}

// Time / data

inline int clock_getres(clockid_t clk_id, struct timespec * tv)
{
    return ::clock_getres(clk_id, tv);
}

inline int clock_gettime(clockid_t clk_id, struct timespec * tv)
{
	return ::clock_gettime(clk_id, tv);
}
inline char const * tzname(tm const & dateTime) { return dateTime.tm_zone; }
inline long int get_timezone(tm const & dateTime) { return dateTime.tm_gmtoff; }
inline tm make_tm(int second, int minute, int hour, int day, int month, int year)
{ return tm { second, minute, hour, day, month - 1, year - 1900, 0, 0, 0, 0, 0 }; }
inline int gettimeofday(struct timeval * time, struct timezone * timeZone)
{
	return ::gettimeofday(time, timeZone);
}
inline void usleep(int64_t usec)
{
	::usleep(usec);
}
inline int nanosleep(const struct timespec *req, struct timespec *rem)
{
	return ::nanosleep(req, rem);
}

// Temp file handling

inline char * Tmpdir()
{
    return P_tmpdir;
}
inline int mkstemp(char * nameTemplate)
{
     return ::mkstemp(nameTemplate);
}

// String handling

inline int strcasecmp(char const * s1, char const * s2)
{
    return ::strcasecmp(s1, s2);
}
inline int wcscasecmp(wchar_t const * s1, wchar_t const * s2)
{
	return ::wcscasecmp(s1, s2);
}

// Environment handling

inline char const * getenv(char const * name)
{
	return ::getenv(name);
}

//typedef struct stat StatStruct;
//
//inline int stat(char const * path, StatStruct & buf)
//{
//    return ::stat(path, &buf);
//}
//inline char * strdup(char const * src)
//{
//    return ::strdup(src);
//}
//inline int rmdir(char const * dir)
//{
//    return ::rmdir(dir);
//}
//inline bool isdir(StatStruct const & st)
//{
//    return S_ISDIR(st.st_mode);
//}
//
//inline char const * strncpy(char * dest, char const * src, size_t n)
//{
//    return ::strncpy(dest, src, n);
//}
//
//inline int chdir(char const * dir)
//{
//    return ::chdir(dir);
//}
//inline FILE * fopen(char const * path, char const * mode)
//{
//    return ::fopen(path, mode);
//}
//inline FILE *freopen(char const * path, char const * mode, FILE * stream)
//{
//    return ::freopen(path, mode, stream);
//}
//inline int read(int fd, void * buf, unsigned int count)
//{
//    return static_cast<int>(::read(fd, buf, count));
//}
//inline int write(int fd, void const * buf, unsigned int count)
//{Platform
//    return static_cast<int>(::write(fd, buf, count));
//}
//inline int close(int fd)
//{
//    return ::close(fd);
//}
//inline char const * getenv(char const * name)
//{
//    return ::getenv(name);
//}
//
//inline void abort()
//{
//    ::abort();
//}
//

}  // namespace OSAL
