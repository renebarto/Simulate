#include "unit-test-c++/UnitTestC++.h"

#include "cocor/TestData.h"

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

TEST_FIXTURE(CocorTest, Construct)
{
    std::string inputFile = "assembler.cocol";
    std::string traceFile = "trace.txt";
    std::string inputDir = TestData::TestDirectoryCocorAssembler();
    std::string inputPath = Core::Path::CombinePath(inputDir, inputFile);
    std::string tracePath = Core::Path::CombinePath(inputDir, traceFile);
    std::string framesDir =TestData::TestDirectoryCocorFrames();

    std::wstring srcDir = Core::String::ToWString(inputDir);
    std::wstring srcName = Core::String::ToWString(inputPath);
    std::wstring traceFileName = Core::String::ToWString(tracePath);
    std::string chTrFileName = tracePath;
    std::wstring nsName = L"Assembler";
    std::wstring frameDir = Core::String::ToWString(framesDir);
    std::wstring outDir = Core::String::ToWString(TestData::TestDirectoryCocorOutput());
    bool emitLines = false;
    std::wstring ddtString = L"AGIPSX";

    Coco::Scanner *scanner = new Coco::Scanner(srcName.c_str());
	Coco::Parser  *parser  = new Coco::Parser(scanner);

    ASSERT_NOT_NULL(parser->trace = fopen(chTrFileName.c_str(), "w"));
	parser->tab  = new Coco::Tab(parser);
	parser->dfa  = new Coco::DFA(parser);
	parser->pgen = new Coco::ParserGen(parser);

	parser->tab->srcName  = coco_string_create(srcName.c_str());
	parser->tab->srcDir   = coco_string_create(srcDir.c_str());
	parser->tab->nsName   = !nsName.empty() ? coco_string_create(nsName.c_str()) : NULL;
	parser->tab->frameDir = coco_string_create(frameDir.c_str());
	parser->tab->outDir   = coco_string_create(!outDir.empty() ? outDir.c_str() : srcDir.c_str());
	parser->tab->emitLines = emitLines;

	if (!ddtString.empty()) 
        parser->tab->SetDDT(ddtString.c_str());

	parser->Parse();

	fclose(parser->trace);

	// obtain the FileSize
	parser->trace = fopen(chTrFileName.c_str(), "r");
	fseek(parser->trace, 0, SEEK_END);
	long fileSize = ftell(parser->trace);
	fclose(parser->trace);
	if (fileSize == 0)
    {
		remove(chTrFileName.c_str());
	} 
    else
    {
		wprintf(L"trace output is in %hs\n", chTrFileName.c_str());
	}

	wprintf(L"%d errors detected\n", parser->errors->count);
    ASSERT_EQ(0, parser->errors->count);

	delete parser->pgen;
	delete parser->dfa;
	delete parser->tab;
	delete parser;
	delete scanner;

    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestTraceRef(), TestData::TestTraceOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestParserHRef(), TestData::TestParserHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestParserCPPRef(), TestData::TestParserCPPOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestScannerHRef(), TestData::TestScannerHOut()));
    EXPECT_TRUE(Core::Util::CompareTextFiles(TestData::TestScannerCPPRef(), TestData::TestScannerCPPOut()));
}

} // namespace Test

} // namespace Simulate
