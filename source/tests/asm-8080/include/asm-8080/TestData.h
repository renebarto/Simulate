#pragma once

#include <string>
#include "core/Path.h"

namespace ASM8080
{

namespace Test
{

class TestData
{
public:
    static const std::string ProjectName() { return "asm-8080"; }
    static const std::string TestProjectName() { return "asm-8080.test"; }

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
    #ifdef _DEBUG
        return Core::Path::CombinePath(BinDirectory(), "debug");
    #else
        return Core::Path::CombinePath(BinDirectory(), "release");
    #endif
    }

    static std::string TestDirectoryASM8080() { return Core::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string InputFile() { return Core::Path::CombinePath(TestDirectoryASM8080(), "input.asm"); }

};

} // namespace Test

} // namespace ASM8080

