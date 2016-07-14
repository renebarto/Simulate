#pragma once

#include <sys/stat.h>
#include <string.h>

// The testing::internal::posix namespace holds wrappers for common
// POSIX functions.  These wrappers hide the differences between
// Windows/MSVC and POSIX systems.  Since some compilers define these
// standard functions as macros, the wrapper cannot have the same name
// as the wrapped function.

namespace posix
{

// Functions with a different name on Windows.

typedef struct stat StatStruct;

inline int FileNo(FILE * file)
{
    return fileno(file);
}
inline int IsATTY(int fd)
{
    return isatty(fd);
}
inline int Stat(char const * path, StatStruct & buf)
{
    return stat(path, &buf);
}
inline int StrCaseCmp(char const * s1, char const * s2)
{
    return strcasecmp(s1, s2);
}
inline char * StrDup(char const * src)
{
    return strdup(src);
}
inline int RmDir(char const * dir)
{
    return rmdir(dir);
}
inline bool IsDir(StatStruct const & st)
{
    return S_ISDIR(st.st_mode);
}

inline char const * StrNCpy(char * dest, char const * src, size_t n)
{
    return strncpy(dest, src, n);
}

// ChDir(), FReopen(), FDOpen(), Read(), Write(), Close(), and
// StrError() aren't needed on Windows CE at this time and thus not
// defined there.

inline int ChDir(char const * dir)
{
    return chdir(dir);
}
inline FILE * FOpen(char const * path, char const * mode)
{
    return fopen(path, mode);
}
inline FILE *FReopen(char const * path, char const * mode, FILE * stream)
{
    return freopen(path, mode, stream);
}
inline FILE * FDOpen(int fd, char const * mode)
{
    return fdopen(fd, mode);
}
inline int FClose(FILE * fp)
{
    return fclose(fp);
}
inline int Read(int fd, void * buf, unsigned int count)
{
    return static_cast<int>(read(fd, buf, count));
}
inline int Write(int fd, void const * buf, unsigned int count)
{
    return static_cast<int>(write(fd, buf, count));
}
inline int Close(int fd)
{
    return close(fd);
}
inline char const * StrError(int errnum)
{
    return strerror(errnum);
}
inline char const * GetEnv(char const * name)
{
    return getenv(name);
}

inline void Abort()
{
    abort();
}

}  // namespace posix

