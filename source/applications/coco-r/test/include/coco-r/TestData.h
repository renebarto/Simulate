#pragma once

#include <string>
#include "core/Path.h"

namespace Coco
{

namespace Test
{

class TestData
{
public:
    static const std::string ProjectName() { return "cocor"; }
    static const std::string TestProjectName() { return "cocor.Test"; }

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

    static std::string TestDirectoryCocor() { return Core::Path::CombinePath(TestDirectory(), ProjectName()); }
    static std::string TestDirectoryCocorOutput() { return Core::Path::InsertPathSeparator(Core::Path::CombinePath(TestDirectoryCocor(), "output"), ""); }
    static std::string TestDirectoryCocorAssembler() { return Core::Path::CombinePath(TestDirectoryCocor(), "assembler"); }
    static std::string TestDirectoryCocorFrames() { return TestDirectoryCocor(); }

};

} // namespace Test

} // namespace Coco

