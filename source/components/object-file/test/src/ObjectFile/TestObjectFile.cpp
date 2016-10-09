#include "unit-test-c++/UnitTestC++.h"

#include "TestData.h"
#include "object-file/ObjectFile.h"
#include "core/Util.h"

using namespace std;

namespace ObjectFile
{

namespace Test
{

static const std::string ModuleName = "Module";

class ObjectFileTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ObjectFileTest::SetUp()
{
}

void ObjectFileTest::TearDown()
{
}

TEST_FIXTURE(ObjectFileTest, Construct)
{
    ObjectFile code;
    // Nothing further to test
}

TEST_FIXTURE(ObjectFileTest, WriteObjectCodeStream)
{
    std::ostringstream stream;
    std::vector<uint8_t> reference{ 0x02, 0x0E, 0x00, 0x06, 'M', 'o', 'd', 'u', 'l', 'e', 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x81,
                                    0x06, 0x04, 0x00, 0x00, 0x00, 0x00, 0xF6, 
                                    0x04, 0x05, 0x00, 0x01, 0x01, 0x00, 0x00, 0xF5,
                                    0x0E, 0x01, 0x00, 0xF1 };
    ObjectFile codeFile;
    ObjectCode code(ModuleName);
    codeFile.WriteObjectCode(stream, code);
    if (!Core::Util::Compare(reference.data(), (uint8_t *)stream.str().data(), stream.str().length()))
    {
        cout << "Expected:" << endl;
        Core::Util::DumpToStream(cout, reference.data(), reference.size());
        cout << "Actual:" << endl;
        Core::Util::DumpToStream(cout, (uint8_t *)stream.str().data(), stream.str().length());
    }
    EXPECT_TRUE(Core::Util::Compare(reference.data(), (uint8_t *)stream.str().data(), stream.str().length()));
}

TEST_FIXTURE(ObjectFileTest, WriteObjectCodePath)
{
    std::ostringstream stream;
    ObjectFile codeFile;
    ObjectCode code(ModuleName);
    codeFile.WriteObjectCode(TestData::TestSimpleOut(), code);
    EXPECT_TRUE(Core::Util::CompareFiles(TestData::TestSimpleOut(), TestData::TestSimple()));
}

} // namespace Test

} // namespace ObjectFile
