#pragma once

#include <string>
#include "core/Path.h"

namespace Assembler
{

namespace Test
{

class TestData
{
public:
    static const std::string ProjectName() { return "assembler"; }
    static const std::string TestProjectName() { return "assembler.Test"; }

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

    static std::string TestDirectoryAssembler() { return Core::Path::CombinePath(TestDirectory(), ProjectName()); }

    static std::string TestNonExistent() { return Core::Path::CombinePath(TestDirectoryAssembler(), "dummy.txt"); }
    static std::string TestASCIISmall() { return Core::Path::CombinePath(TestDirectoryAssembler(), "ASCIISmall.txt"); }
    static std::string TestUTF8Small() { return Core::Path::CombinePath(TestDirectoryAssembler(), "UTF8Small.txt"); }
    static std::string TestUTF8SmallBOM() { return Core::Path::CombinePath(TestDirectoryAssembler(), "UTF16SmallBOM.txt"); }
};

} // namespace Test

} // namespace Assembler

