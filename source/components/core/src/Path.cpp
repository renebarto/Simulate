#include "core/Path.h"

#include <climits>
#include <cstring>
#include <sys/stat.h>
#include "core/Util.h"

using namespace std;
using namespace Core;

#if defined (__linux__)
static char PathSeparator = '/';
#elif defined(_WIN32)
static char PathSeparator = '\\';
#endif

char Path::PathSeparator()
{
	return ::PathSeparator;
}

std::string Path::InsertPathSeparator(std::string const & firstPart, std::string const & secondPart)
{
	return firstPart + PathSeparator() + secondPart;
}

bool Path::FileExists(string const & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISREG(status.st_mode));
}

bool Path::DirectoryExists(string const & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    return (S_ISDIR(status.st_mode));
}

void Path::MakeSureFileDoesNotExist(string const & path)
{
    if (FileExists(path))
    {
        OSAL::unlink(path.c_str());
    }
}

void Path::MakeSureDirectoryExists(string const & path)
{
    struct stat status;
    memset(&status, 0, sizeof(status));
    stat(path.c_str(), &status);
    if (S_ISDIR(status.st_mode))
        return;
    if (S_ISREG(status.st_mode))
    {
		OSAL::unlink(path.c_str());
    }
	OSAL::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Path::SplitPath(string const & path, string & directory, string & fileName)
{
    directory = {};
    fileName = {};
    if (DirectoryExists(path))
    {
        directory = path;
        return;
    }
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != string::npos)
    {
        fileName = path.substr(lastPathDelimiterPos + 1);
        directory = path.substr(0, lastPathDelimiterPos);
        return;
    }
    fileName = path.substr(0, lastPathDelimiterPos);
}

string Path::Extension(string const & path)
{
    size_t lastPathDelimiterPos = path.find_last_of('.');
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(lastPathDelimiterPos);
    }
    return string();
}

string Path::StripExtension(string const & path)
{
    size_t lastPathDelimiterPos = path.find_last_of('.');
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(0, lastPathDelimiterPos);
    }
    return path;
}

string Path::LastPartOfPath(string const & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(lastPathDelimiterPos + 1);
    }
    return path;
}

bool Path::HasNoPath(string const & path)
{
    string directory;
    string fileName;
    Path::SplitPath(path, directory, fileName);
    return directory.empty();
}

string Path::CreatePathWithDefault(string const & pathOrFileName, string const & defaultPath)
{
    if (HasNoPath(pathOrFileName))
    {
        return CombinePath(defaultPath, pathOrFileName);
    }
    return pathOrFileName;
}

string Path::CombinePath(string const & basePath, string const & subPath)
{
    string result = basePath;
    if (result.empty())
        result += ".";
    if (result[result.length() - 1] != PathSeparator())
        result += PathSeparator();
    result += subPath;
    return result;
}

string Path::ResolveTilde(string const & path)
{
    if (path.length() < 2)
        return path;
    if (path.substr(0, 2) != "~/")
        return path;
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
	return CombinePath(string(getenv("HOME")), path.substr(2));
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
}

string Path::FullPath(string const & path)
{
    char buffer[PATH_MAX];
    string resolvedPath = ResolveTilde(path);
    const char * fullpath = OSAL::realpath(resolvedPath.c_str(), buffer);
    if (fullpath == nullptr)
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return string(fullpath);
}

void AddSlashIfNeeded(string & path)
{
    if ((path.length() > 0) && (path[path.length() - 1] != PathSeparator))
        path += PathSeparator;
}

string Path::RelativePath(string const & path)
{
    string currentDir = CurrentDir();
    string fullPath = FullPath(path);
    size_t index = 0;
    while ((index < currentDir.length()) && (index < fullPath.length()) && (currentDir[index] == fullPath[index]))
        index++;
    string relativePath = "";
	// All paths start with / on linux, or with drive on Windows
	// If drive is different, we see this as a different abs path
#if defined(__linux__)
	if (index == 1)
#elif defined(_WIN32)
	if (index == 0)
#endif
    {
        relativePath = fullPath;
    }
    else
    {
        currentDir = currentDir.substr(index);
        fullPath = fullPath.substr(index);
        if (currentDir.empty())
        {
            relativePath = ".";
            if ((fullPath.length() > 0) && (fullPath[0] == PathSeparator()))
                fullPath = fullPath.substr(1); // Split was before /
        }
        else
        {
            size_t pathDelimiterPos = string::npos;
            do
            {
                pathDelimiterPos = currentDir.find_last_of(PathSeparator());
                if (pathDelimiterPos != string::npos)
                {
                    currentDir = currentDir.substr(0, pathDelimiterPos);
                    AddSlashIfNeeded(relativePath);
                    relativePath += "..";
                }
            }
            while (pathDelimiterPos != string::npos);
            if (!currentDir.empty())
            {
                AddSlashIfNeeded(relativePath);
                relativePath += "..";
            }
        }
        if (!fullPath.empty())
        {
            AddSlashIfNeeded(relativePath);
            relativePath += fullPath;
        }
    }

    return relativePath;
}

string Path::CurrentDir()
{
    char * currentDirectory = OSAL::get_current_dir_name();
    if (currentDirectory == nullptr)
    {
        Util::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    string result(currentDirectory);
    free(currentDirectory);
    return FullPath(result);
}

string Path::StripPathToSubDirectory(string const & path, string const & subDirectoryName)
{
    string strippedPath = path;
    bool done = false;
    while (!done)
    {
		size_t index = strippedPath.find_last_of(PathSeparator());
        if (index != string::npos)
        {
            if (strippedPath.substr(index + 1) == subDirectoryName)
            {
                done = true;
            }
            else
            {
                strippedPath = strippedPath.substr(0, index);
            }
        }
        else
        {
            return "";
        }
    }
    return strippedPath;
}
