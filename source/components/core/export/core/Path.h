#pragma once

#include <string>

#include "osal.h"

namespace Core
{

namespace Path
{

char PathSeparator();
std::string InsertPathSeparator(std::string const & firstPart, std::string const & secondPart);
bool FileExists(std::string const & path);
bool DirectoryExists(std::string const & path);
void MakeSureDirectoryExists(std::string const & path);
void MakeSureFileDoesNotExist(std::string const & path);
std::string ResolveTilde(std::string const & path);
void SplitPath(std::string const & path, std::string & directory, std::string & fileName);
std::string Extension(std::string const & path);
std::string StripExtension(std::string const & path);
std::string LastPartOfPath(std::string const & path);
bool HasNoPath(std::string const & path);
std::string CreatePathWithDefault(std::string const & pathOrFileName, std::string const & defaultPath);
std::string CombinePath(std::string const & basePath, std::string const & subPath);
std::string FullPath(std::string const & path);
std::string RelativePath(std::string const & path);
std::string CurrentDir();
std::string StripPathToSubDirectory(std::string const & path, std::string const & subDirectoryName);

class TemporaryFile
{
public:
    TemporaryFile()
    {
		std::string fileMaskString = Path::CombinePath(OSAL::Tmpdir(), "testXXXXXX");
        char fileMask[MAX_PATH];
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
		strcpy(fileMask, fileMaskString.c_str());
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
		int fd = OSAL::mkstemp(fileMask);
        path = fileMask;
        fp = OSAL::fdopen(fd, "w");
    }
    ~TemporaryFile()
    {
        Close();
        MakeSureFileDoesNotExist(path);
    }
    void Close()
    {
        if (fp)
			OSAL::fclose(fp);
        fp = nullptr;
    }
    FILE * GetFP()
    {
        return fp;
    }
    std::string const & GetPath() const
    {
        return path;
    }
private:
    FILE * fp;
    std::string path;
};

} // namespace Path

} // namespace Core

