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
    static const std::string ProjectName() { return "coco-r"; }
    static const std::string TestProjectName() { return "coco-r.Test"; }

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
    static std::string TestDirectoryCocorOutput() { return Core::Path::CombinePath(TestDirectoryCocor(), "output"); }
    static std::string TestDirectoryCocorAssembler() { return Core::Path::CombinePath(TestDirectoryCocor(), "assembler"); }
    static std::string TestDirectoryCocorFrames() { return TestDirectoryCocor(); }
    static std::string TestDirectoryCocorRef() { return Core::Path::CombinePath(TestDirectoryCocor(), "ref"); }
    static std::string TestDirectoryCocorOutputAssembler() { return Core::Path::InsertPathSeparator(Core::Path::CombinePath(TestDirectoryCocorOutput(), "assembler"), ""); }
    static std::string TestDirectoryCocorRefAssembler() { return Core::Path::CombinePath(TestDirectoryCocorRef(), "assembler"); }
    static std::string TestDirectoryCocorOutputCocol() { return Core::Path::InsertPathSeparator(Core::Path::CombinePath(TestDirectoryCocorOutput(), "cocol"), ""); }
    static std::string TestDirectoryCocorRefCocol() { return Core::Path::CombinePath(TestDirectoryCocorRef(), "cocol"); }
    static std::string TestTraceFileName() { return "trace.txt"; }
    static std::string TestParserHFileName() { return "Parser.h"; }
    static std::string TestParserCPPFileName() { return "Parser.cpp"; }
    static std::string TestScannerHFileName() { return "Scanner.h"; }
    static std::string TestScannerCPPFileName() { return "Scanner.cpp"; }

    static std::string TestAssemblerTraceRef() { return Core::Path::CombinePath(TestDirectoryCocorRefAssembler(), TestTraceFileName()); }
    static std::string TestAssemblerParserHRef() { return Core::Path::CombinePath(TestDirectoryCocorRefAssembler(), TestParserHFileName()); }
    static std::string TestAssemblerParserCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRefAssembler(), TestParserCPPFileName()); }
    static std::string TestAssemblerScannerHRef() { return Core::Path::CombinePath(TestDirectoryCocorRefAssembler(), TestScannerHFileName()); }
    static std::string TestAssemblerScannerCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRefAssembler(), TestScannerCPPFileName()); }
    static std::string TestAssemblerTraceOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputAssembler(), TestTraceFileName()); }
    static std::string TestAssemblerParserHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputAssembler(), TestParserHFileName()); }
    static std::string TestAssemblerParserCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputAssembler(), TestParserCPPFileName()); }
    static std::string TestAssemblerScannerHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputAssembler(), TestScannerHFileName()); }
    static std::string TestAssemblerScannerCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputAssembler(), TestScannerCPPFileName()); }

    static std::string TestCocolTraceRef() { return Core::Path::CombinePath(TestDirectoryCocorRefCocol(), TestTraceFileName()); }
    static std::string TestCocolParserHRef() { return Core::Path::CombinePath(TestDirectoryCocorRefCocol(), TestParserHFileName()); }
    static std::string TestCocolParserCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRefCocol(), TestParserCPPFileName()); }
    static std::string TestCocolScannerHRef() { return Core::Path::CombinePath(TestDirectoryCocorRefCocol(), TestScannerHFileName()); }
    static std::string TestCocolScannerCPPRef() { return Core::Path::CombinePath(TestDirectoryCocorRefCocol(), TestScannerCPPFileName()); }
    static std::string TestCocolTraceOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputCocol(), TestTraceFileName()); }
    static std::string TestCocolParserHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputCocol(), TestParserHFileName()); }
    static std::string TestCocolParserCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputCocol(), TestParserCPPFileName()); }
    static std::string TestCocolScannerHOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputCocol(), TestScannerHFileName()); }
    static std::string TestCocolScannerCPPOut() { return Core::Path::CombinePath(TestDirectoryCocorOutputCocol(), TestScannerCPPFileName()); }
};

} // namespace Test

} // namespace Coco

