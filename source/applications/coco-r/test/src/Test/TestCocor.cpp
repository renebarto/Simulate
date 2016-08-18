#include "unit-test-c++/UnitTestC++.h"

#include "coco-r/TestData.h"

#include "core/String.h"
#include "core/Util.h"
#include "Scanner.h"
#include "Parser.h"

using namespace std;

namespace Coco
{

namespace Test
{

class CocorTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void CocorTest::SetUp()
{
}

void CocorTest::TearDown()
{
}

TEST_FIXTURE(CocorTest, Assembler)
{
    std::string inputFile = "assembler.atg";
    std::string traceFile = "trace.txt";
    std::string inputDir = TestData::TestDirectoryCocor();
    std::string inputPath = Core::Path::CombinePath(inputDir, inputFile);
    std::string tracePath = TestData::TestAssemblerTraceOut();
    std::string framesDir = TestData::TestDirectoryCocorFrames();

    std::string nsName = "Assembler";
    std::string outDir = TestData::TestDirectoryCocorOutputAssembler();
    bool emitLines = false;
    std::wstring ddtString = L"AGIPSX";

    Core::Path::MakeSureDirectoryExists(TestData::TestDirectoryCocorOutputAssembler());

    Coco::Scanner *scanner = new Coco::Scanner(inputPath);
	Coco::Parser  *parser  = new Coco::Parser(scanner);

    ASSERT_NOT_NULL(parser->trace = fopen(tracePath.c_str(), "w"));
	parser->tab  = new Coco::Tab(parser);
	parser->dfa  = new Coco::DFA(parser);
	parser->pgen = new Coco::ParserGen(parser);

	parser->tab->srcName  = inputPath;
	parser->tab->srcDir   = inputDir;
	parser->tab->nsName   = nsName;
	parser->tab->frameDir = framesDir;
	parser->tab->outDir   = !outDir.empty() ? outDir : inputDir;
	parser->tab->emitLines = emitLines;

	if (!ddtString.empty()) 
        parser->tab->SetDDT(ddtString.c_str());

	parser->Parse();

	fclose(parser->trace);

	// obtain the FileSize
	parser->trace = fopen(tracePath.c_str(), "r");
	fseek(parser->trace, 0, SEEK_END);
	long fileSize = ftell(parser->trace);
	fclose(parser->trace);
	if (fileSize == 0)
    {
		remove(tracePath.c_str());
	} 
    else
    {
		wprintf(L"trace output is in %hs\n", tracePath.c_str());
	}

	wprintf(L"%d errors detected\n", parser->errors->count);
    ASSERT_EQ(0, parser->errors->count);

	delete parser->pgen;
	delete parser->dfa;
	delete parser->tab;
	delete parser;
	delete scanner;

    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestAssemblerTraceRef(), TestData::TestAssemblerTraceOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestAssemblerParserHRef(), TestData::TestAssemblerParserHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestAssemblerParserCPPRef(), TestData::TestAssemblerParserCPPOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestAssemblerScannerHRef(), TestData::TestAssemblerScannerHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestAssemblerScannerCPPRef(), TestData::TestAssemblerScannerCPPOut()));
}

TEST_FIXTURE(CocorTest, Cocol)
{
    std::string inputFile = "cocol.atg";
    std::string traceFile = "trace.txt";
    std::string inputDir = TestData::TestDirectoryCocor();
    std::string inputPath = Core::Path::CombinePath(inputDir, inputFile);
    std::string tracePath = TestData::TestCocolTraceOut();
    std::string framesDir = TestData::TestDirectoryCocorFrames();

    std::string nsName = "Assembler";
    std::string outDir = TestData::TestDirectoryCocorOutputCocol();
    bool emitLines = false;
    std::wstring ddtString = L"AGIPSX";

    Core::Path::MakeSureDirectoryExists(TestData::TestDirectoryCocorOutputCocol());

    Coco::Scanner *scanner = new Coco::Scanner(inputPath);
	Coco::Parser  *parser  = new Coco::Parser(scanner);

    ASSERT_NOT_NULL(parser->trace = fopen(tracePath.c_str(), "w"));
	parser->tab  = new Coco::Tab(parser);
	parser->dfa  = new Coco::DFA(parser);
	parser->pgen = new Coco::ParserGen(parser);

	parser->tab->srcName  = inputPath;
	parser->tab->srcDir   = inputDir;
	parser->tab->nsName   = nsName;
	parser->tab->frameDir = framesDir;
	parser->tab->outDir   = !outDir.empty() ? outDir : inputDir;
	parser->tab->emitLines = emitLines;

	if (!ddtString.empty()) 
        parser->tab->SetDDT(ddtString.c_str());

	parser->Parse();

	fclose(parser->trace);

	// obtain the FileSize
	parser->trace = fopen(tracePath.c_str(), "r");
	fseek(parser->trace, 0, SEEK_END);
	long fileSize = ftell(parser->trace);
	fclose(parser->trace);
	if (fileSize == 0)
    {
		remove(tracePath.c_str());
	} 
    else
    {
		wprintf(L"trace output is in %hs\n", tracePath.c_str());
	}

	wprintf(L"%d errors detected\n", parser->errors->count);
    ASSERT_EQ(0, parser->errors->count);

	delete parser->pgen;
	delete parser->dfa;
	delete parser->tab;
	delete parser;
	delete scanner;

    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestCocolTraceRef(), TestData::TestCocolTraceOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestCocolParserHRef(), TestData::TestCocolParserHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestCocolParserCPPRef(), TestData::TestCocolParserCPPOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestCocolScannerHRef(), TestData::TestCocolScannerHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestCocolScannerCPPRef(), TestData::TestCocolScannerCPPOut()));
}

} // namespace Test

} // namespace Simulate
