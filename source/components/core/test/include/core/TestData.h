#pragma once

#include <string>
#include "core/Path.h"

namespace Core
{

namespace Test
{

extern std::string BuildLogPath(const std::string & componentName, const std::string & fileName);

class TestData
{
public:
    static const std::string ProjectName() { return "core"; }
    static const std::string TestProjectName() { return "core.Test"; }

    static std::string SourceDirectory()
    {
        return Path::StripPathToSubDirectory(Path::CurrentDir(), "source");
    }
    static std::string TestDirectory()
    {
        return Path::CombinePath(SourceDirectory(), Path::InsertPathSeparator("..", "testdata"));
    }
    static std::string BinDirectory()
    {
        return Path::CombinePath(SourceDirectory(), Path::InsertPathSeparator("..", "bin"));
    }
    static std::string BinariesDirectory()
    {
    #ifdef DEBUG
        return Path::CombinePath(BinDirectory(), "debug");
    #else
        return Path::CombinePath(BinDirectory(), "release");
    #endif
    }

    static std::string TestDirectoryCore() { return Path::CombinePath(TestDirectory(), ProjectName()); }
    static std::string TestLogDirectoryCore() { return Path::CombinePath(Path::CurrentDir(), "log"); }
    static std::string LogFileRef() { return Path::CombinePath(TestDirectoryCore(), "log.File.ref.txt"); }
    static std::string LogStreamRef() { return Path::CombinePath(TestDirectoryCore(), "log.Stream.ref.txt"); }

    static std::string FilledDirName() { return "FilledDir"; }
    static std::string FilledDirPath() { return Path::CombinePath(TestDirectoryCore(), FilledDirName()); }
    static std::string TstSubDirName() { return "TstSubDir"; }
    static std::string TstSubDirPath() { return Path::CombinePath(FilledDirPath(), TstSubDirName()); }
    static std::string TstSubSubDirName() { return "TstSubSubDir"; }
    static std::string TstSubSubDirPath() { return Path::CombinePath(TstSubDirPath(), TstSubSubDirName()); }
    static std::string EmptyDirPath() { return Path::CombinePath(TestDirectoryCore(), "EmptyDir"); }
    static std::string HiddenDirPath() { return Path::CombinePath(FilledDirPath(), "HiddenDir"); }
    static std::string DummyDirName() { return "Dummy"; }
    static std::string DummyDirPath() { return Path::CombinePath(TestDirectoryCore(), DummyDirName()); }

    static std::string RegularFileNameNoExtension() { return "a"; }
    static std::string RegularFileExtension() { return ".tst"; }
    static std::string RegularFileName() { return RegularFileNameNoExtension() + RegularFileExtension(); }
    static std::string RegularFilePath() { return Path::CombinePath(FilledDirPath(), RegularFileName()); }
    static std::string HiddenFileName() { return ".hidden.txt"; }
    static std::string HiddenFilePath() { return Path::CombinePath(FilledDirPath(), HiddenFileName()); }
    static std::string DummyFileName() { return "dummy.txt"; }
    static std::string DummyFilePath() { return Path::CombinePath(FilledDirPath(), DummyFileName()); }

};

} // namespace Test

} // namespace Core

