#include "unit-test-c++/UnitTestC++.h"
#include "core/CommandLineOptionGroup.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class CommandLineOptionGroupTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    int theParameter;
    bool optionFound;
};


void CommandLineOptionGroupTest::SetUp()
{
}

void CommandLineOptionGroupTest::TearDown()
{
}

static const string GroupName = "group";
static const string GroupDescription = "Group description";
static const string GroupName2 = "group2";
static const string GroupDescription2 = "Group2 description";

static const CommandLineArgumentType ArgType = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgTypeRequired = CommandLineArgumentType::RequiredArgument;
static const CommandLineArgumentType ArgTypeOptional = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";
static const char ShortName2 = 't';
static const string LongName2 = "othername";
static const string Description2 = "Some other description";

TEST_FIXTURE(CommandLineOptionGroupTest, Construction)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    EXPECT_EQ(GroupName, optionGroup.Name());
    EXPECT_EQ(GroupDescription, optionGroup.Description());
    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, GetSet)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.Name(GroupName2);
    optionGroup.Description(GroupDescription2);

    EXPECT_EQ(GroupName2, optionGroup.Name());
    EXPECT_EQ(GroupDescription2, optionGroup.Description());
    EXPECT_EQ(size_t(0), optionGroup.Options().size());
}

TEST_FIXTURE(CommandLineOptionGroupTest, Add)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);
    CommandLineOptionPtr option1(new CommandLineOption(LongName, ShortName, Description));
    CommandLineOptionPtr option2(new CommandLineOptionWithArgument<int>(LongName2, ShortName2,
                                                                        Description2, &theParameter));

    optionGroup.Add(option1);
    optionGroup.Add(option2);

    EXPECT_EQ(GroupName, optionGroup.Name());
    EXPECT_EQ(GroupDescription, optionGroup.Description());
    EXPECT_EQ(size_t(2), optionGroup.Options().size());
    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[1]->ArgType());
    EXPECT_EQ(LongName2, optionGroup.Options()[1]->LongName());
    EXPECT_EQ(ShortName2, optionGroup.Options()[1]->ShortName());
    EXPECT_EQ(Description2, optionGroup.Options()[1]->Description());
    EXPECT_NOT_NULL(dynamic_cast<const CommandLineOptionWithArgument<int> *>(optionGroup.Options()[1].get()));
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionNoArgument)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionNoArgument(LongName, ShortName, Description);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    EXPECT_NULL(optionGroup.Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionNoArgumentOptionFoundFlag)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionNoArgument(LongName, ShortName, Description, &optionFound);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgType, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    EXPECT_EQ(&optionFound, optionGroup.Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgument)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionRequiredArgument(LongName, ShortName, Description, &theParameter);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(optionGroup.Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theParameter, option->Argument());
    EXPECT_NULL(optionGroup.Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionRequiredArgumentOptionFoundFlag)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionRequiredArgument(LongName, ShortName, Description, &optionFound, &theParameter);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgTypeRequired, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(optionGroup.Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theParameter, option->Argument());
    EXPECT_EQ(&optionFound, optionGroup.Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgument)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionOptionalArgument(LongName, ShortName, Description, &theParameter);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgTypeOptional, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(optionGroup.Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theParameter, option->Argument());
    EXPECT_NULL(optionGroup.Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineOptionGroupTest, AddOptionOptionalArgumentOptionFoundFlag)
{
    CommandLineOptionGroup optionGroup(GroupName, GroupDescription);

    optionGroup.AddOptionOptionalArgument(LongName, ShortName, Description, &optionFound, &theParameter);
    EXPECT_EQ(size_t(1), optionGroup.Options().size());
    EXPECT_EQ(ArgTypeOptional, optionGroup.Options()[0]->ArgType());
    EXPECT_EQ(LongName, optionGroup.Options()[0]->LongName());
    EXPECT_EQ(ShortName, optionGroup.Options()[0]->ShortName());
    EXPECT_EQ(Description, optionGroup.Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(optionGroup.Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theParameter, option->Argument());
    EXPECT_EQ(&optionFound, optionGroup.Options()[0]->OptionFoundFlag());
}

} // namespace Test

} // namespace Core
