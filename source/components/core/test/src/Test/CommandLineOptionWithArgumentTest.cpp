#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOption.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class CommandLineOptionWithArgumentTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    int theParameter;
    int theParameter2;
    bool optionFound;
};

void CommandLineOptionWithArgumentTest::SetUp()
{
}

void CommandLineOptionWithArgumentTest::TearDown()
{
}

static const CommandLineArgumentType ArgTypeRequired = CommandLineArgumentType::RequiredArgument;
static const CommandLineArgumentType ArgTypeOptional = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_FIXTURE(CommandLineOptionWithArgumentTest, ConstructionRequired)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &theParameter);

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_NULL(option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, ConstructionRequiredOptionFoundFlag)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &optionFound, &theParameter);

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, ConstructionOptional)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &theParameter, false);

    EXPECT_EQ(ArgTypeOptional, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_NULL(option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, ConstructionOptionalOptionFoundFlag)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &optionFound, &theParameter, false);

    EXPECT_EQ(ArgTypeOptional, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, ConstructionCopy)
{
    CommandLineOptionWithArgument<int> other(LongName, ShortName, Description, &optionFound, &theParameter);
    CommandLineOptionWithArgument<int> option(other);

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, Assignment)
{
    CommandLineOptionWithArgument<int> other(LongName, ShortName, Description, &optionFound, &theParameter);
    CommandLineOptionWithArgument<int> option(LongName2, ShortName2, Description2, &theParameter2, false);

    EXPECT_EQ(ArgTypeOptional, option.ArgType());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(&theParameter2, option.Argument());
    EXPECT_NULL(option.OptionFoundFlag());

    option = other;

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(LongName, option.LongName());
    EXPECT_EQ(ShortName, option.ShortName());
    EXPECT_EQ(Description, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, GetSet)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &theParameter);

    option.LongName(LongName2);
    option.ShortName(ShortName2);
    option.Description(Description2);
    option.OptionFoundFlag(&optionFound);

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_FALSE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, FoundOption)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &theParameter);

    option.LongName(LongName2);
    option.ShortName(ShortName2);
    option.Description(Description2);
    option.OptionFoundFlag(&optionFound);
    option.FoundOption(true);

    EXPECT_EQ(ArgTypeRequired, option.ArgType());
    EXPECT_EQ(ShortName2, option.ShortName());
    EXPECT_EQ(LongName2, option.LongName());
    EXPECT_EQ(Description2, option.Description());
    EXPECT_EQ(&theParameter, option.Argument());
    EXPECT_EQ(&optionFound, option.OptionFoundFlag());
    EXPECT_TRUE(option.FoundOption());
}

TEST_FIXTURE(CommandLineOptionWithArgumentTest, Value)
{
    CommandLineOptionWithArgument<int> option(LongName, ShortName, Description, &theParameter);

    const int expected = 100;
    const string value = "100";
    option.Value(value);

    EXPECT_EQ(expected, theParameter);
}

} // namespace Test

} // namespace Core
