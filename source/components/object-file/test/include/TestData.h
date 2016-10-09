#pragma once

#include <string>
#include "core/Path.h"

namespace ObjectFile
{

namespace Test
{

class TestData
{
public:
    static const std::string ProjectName() { return "object-file"; }
    static const std::string TestProjectName() { return "object-file.test"; }

    static std::string SourceDirectory()
    {
        return Core::Path::StripPathToSubDirectory(Core::Path::CurrentDir(), "source");
    }
    static std::string TestDirectory()
    {
        return Core::Path::CombinePath(SourceDirectory(), Core::Path::InsertPathSeparator("..", "testdata"));
    }
    static std::string BinDirectory()
    {
        return Core::Path::CombinePath(SourceDirectory(), Core::Path::InsertPathSeparator("..", "bin"));
    }
    static std::string BinariesDirectory()
    {
    #ifdef DEBUG
        return Path::CombinePath(BinDirectory(), "debug");
    #else
        return Core::Path::CombinePath(BinDirectory(), "release");
    #endif
    }

    static std::string TestDirectoryObjectFile() { return Core::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string TestNonExistent() { return Core::Path::CombinePath(TestDirectoryObjectFile(), "dummy.o"); }
    static std::string TestSimple() { return Core::Path::CombinePath(TestDirectoryObjectFile(), "simple.o"); }
    static std::string TestSimpleOut() { return Core::Path::CombinePath(TestDirectoryObjectFile(), "simple.out.o"); }
};

} // namespace Test

} // namespace ObjectFile

