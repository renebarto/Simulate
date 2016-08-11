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
    static std::string TestDirectoryCocorRef() { return Core::Path::CombinePath(TestDirectoryCocor(), "ref"); }
    static std::string TestTraceFileName() { return "trace.txt"; }
    static std::string TestParserHFileName() { return "Parser.h"; }
    static std::string TestParserCPPFileName() { return "Parser.cpp"; }
    static std::string TestScannerHFileName() { return "Scanner.h"; }
    static std::string TestScannerCPPFileName() { return "Scanner.cpp"; }
    static std::string TestTraceRef() { return Core::Path::CombinePath(TestDirectoryCocorRef(), TestTraceFileName()); }
    static std::string TestParserHRef() { return Core::Path::CombinePath(TestDirectoryCocorRef(), TestParserHFileName()); }
    static std::string TestParserCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRef(), TestParserCPPFileName()); }
    static std::string TestScannerHRef() { return Core::Path::CombinePath(TestDirectoryCocorRef(), TestScannerHFileName()); }
    static std::string TestScannerCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRef(), TestScannerCPPFileName()); }
    static std::string TestTraceOut() { return Core::Path::CombinePath(TestDirectoryCocorOutput(), TestTraceFileName()); }
    static std::string TestParserHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutput(), TestParserHFileName()); }
    static std::string TestParserCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutput(), TestParserCPPFileName()); }
    static std::string TestScannerHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutput(), TestScannerHFileName()); }
    static std::string TestScannerCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutput(), TestScannerCPPFileName()); }
};

} // namespace Test

} // namespace Coco

