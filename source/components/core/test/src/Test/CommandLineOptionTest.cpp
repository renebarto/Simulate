#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class CommandLineOptionTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    bool optionFound;
};

void CommandLineOptionTest::SetUp()
{
}

void CommandLineOptionTest::TearDown()
{
}

static const CommandLineArgumentType ArgTypeDefault = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgType = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const CommandLineArgumentType ArgType2 = CommandLineArgumentType::RequiredArgument;
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_FIXTURE(CommandLineOptionTest, Construction)
{
    CommandLineOption option(LongName, ShortName, Description);

    EXPECT_EQ(ArgTypeDefault, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_NULL(option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionFoundFlag)
{
    CommandLineOption option(LongName, ShortName, Description, &optionFound);

    EXPECT_EQ(ArgTypeDefault, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(optionFound);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionArgType)
{
    CommandLineOption option(LongName, ShortName, Description, ArgType);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_NULL(option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionOptionFoundFlagArgType)
{
    CommandLineOption option(LongName, ShortName, Description, &optionFound, ArgType);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(optionFound);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, ConstructionCopy)
{
    CommandLineOption other(LongName, ShortName, Description, &optionFound, ArgType);
    CommandLineOption option(other);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(optionFound);
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, Assignment)
{
    CommandLineOption other(LongName, ShortName, Description, &optionFound, ArgType);
    CommandLineOption option(LongName2, ShortName2, Description2, ArgType2);

    EXPECT_EQ(ArgType2, option.ArgType());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_NULL(option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());

    option = other;

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, GetSet)
{
    CommandLineOption option(LongName, ShortName, Description, ArgType);

    option.LongName(LongName2);
    option.ShortName(ShortName2);
    option.Description(Description2);
    option.OptionFoundFlag(&optionFound);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, FoundOption)
{
    CommandLineOption option(LongName, ShortName, Description, ArgType);

    option.LongName(LongName2);
    option.ShortName(ShortName2);
    option.Description(Description2);
    option.OptionFoundFlag(&optionFound);
    option.FoundOption(true);

    EXPECT_EQ(ArgType, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_TRUE(optionFound);
    EXPECT_TRUE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionTest, Value)
{
    CommandLineOption option(LongName, ShortName, Description, ArgType);

    const string expected = "100";
    option.Value(expected);

    EXPECT_EQ(expected, option.Value());
}

} // namespace Test

} // namespace Core
