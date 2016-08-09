#include "unit-test-c++/UnitTestC++.h"
#include <cstring>
#include "core/CommandLineParser.h"
#include "core/Util.h"
#include "core/String.h"

using namespace std;

namespace Core
{

namespace Test
{

class CommandLineParserWithCallback : public CommandLineParser
{
public:
    string runSuite;
    string runFixture;
    string runTest;
    bool xmlOutput = false;

    CommandLineParserWithCallback(const string & name = DefaultMainOptionsGroupName, const string & description = "") :
        CommandLineParser(name, description)
    {

    }

    virtual void OnParseOption(const CommandLineOptionPtr option)
    {
        if (Core::String::IsEqualIgnoreCase(option->LongName(), "runsuite"))
        {
            runSuite = option->Value();
        }
        else if (Core::String::IsEqualIgnoreCase(option->LongName(), "runfixture"))
        {
            runFixture = option->Value();
        }
        else if (Core::String::IsEqualIgnoreCase(option->LongName(), "runtest"))
        {
            runTest = option->Value();
        }
        else if (Core::String::IsEqualIgnoreCase(option->LongName(), "xmloutput"))
        {
            xmlOutput = option->FoundOption();
        }

    }
};

class CommandLineParserTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    void AddCommandLineParameter(int & argc, char * const * & argv, const char * str);
    void DestroyCommandLineParameterList(int & argc, char * const * & argv);

    int theArgument;
    bool optionFound;
};

void CommandLineParserTest::SetUp()
{
}

void CommandLineParserTest::TearDown()
{
}

void CommandLineParserTest::AddCommandLineParameter(int & argc, char * const * & argv, const char * str)
{
    char ** argvNew = new char *[argc + 2];
    if (argc >  0)
        memcpy(argvNew, argv, argc * sizeof(char *));
    delete [] argv;

    argvNew[argc] = new char[strlen(str) + 1];
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
	strcpy(argvNew[argc], str);
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
	argc++;
    argvNew[argc] = nullptr;
    argv = argvNew;
}

void CommandLineParserTest::DestroyCommandLineParameterList(int & argc, char * const * & argv)
{
    if (argv != nullptr)
    {
        for (int i = 0; i < argc; i++)
        {
            delete [] argv[i];
        }
        delete [] argv;
    }
    argc = 0;
    argv = nullptr;
}

static const string MainGroupName = "maingroup";
static const string MainGroupDescription = "Main group description";
static const string GroupName = "group";
static const string GroupDescription = "Group description";
static const string GroupNameApplications = "Application Options";
static const string GroupDescriptionApplications = "";
static const CommandLineArgumentType ArgType = CommandLineArgumentType::NoArgument;
static const CommandLineArgumentType ArgTypeRequired = CommandLineArgumentType::RequiredArgument;
static const CommandLineArgumentType ArgTypeOptional = CommandLineArgumentType::OptionalArgument;
static const char ShortName = 's';
static const string LongName = "longname";
static const string Description = "Some description";

TEST_FIXTURE(CommandLineParserTest, ConstructionDefault)
{
    CommandLineParser parser;

    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(CommandLineParser::DefaultMainOptionsGroupName, parser.Groups()[0]->Name());
    EXPECT_EQ("", parser.Groups()[0]->Description());
    EXPECT_EQ(CommandLineParser::HelpOptionsGroupName, parser.Groups()[1]->Name());
    EXPECT_EQ("", parser.Groups()[1]->Description());
}

TEST_FIXTURE(CommandLineParserTest, ConstructionName)
{
    CommandLineParser parser(MainGroupName);

    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(MainGroupName, parser.Groups()[0]->Name());
    EXPECT_EQ("", parser.Groups()[0]->Description());
    EXPECT_EQ(CommandLineParser::HelpOptionsGroupName, parser.Groups()[1]->Name());
    EXPECT_EQ("", parser.Groups()[1]->Description());
}

TEST_FIXTURE(CommandLineParserTest, ConstructionNameDescription)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(MainGroupName, parser.Groups()[0]->Name());
    EXPECT_EQ(MainGroupDescription, parser.Groups()[0]->Description());
    EXPECT_EQ(CommandLineParser::HelpOptionsGroupName, parser.Groups()[1]->Name());
    EXPECT_EQ("", parser.Groups()[1]->Description());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionNoArgument(LongName, ShortName, Description);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgType, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    EXPECT_NULL(parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionNoArgumentOptionFound)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionNoArgument(LongName, ShortName, Description, &optionFound);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgType, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    EXPECT_EQ(&optionFound, parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionRequiredArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionRequiredArgument(LongName, ShortName, Description, &theArgument);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgTypeRequired, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(parser.MainGroup()->Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theArgument, option->Argument());
    EXPECT_NULL(parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionRequiredArgumentOptionFound)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionRequiredArgument(LongName, ShortName, Description, &optionFound, &theArgument);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgTypeRequired, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(parser.MainGroup()->Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theArgument, option->Argument());
    EXPECT_EQ(&optionFound, parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionOptionalArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionOptionalArgument(LongName, ShortName, Description, &theArgument);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgTypeOptional, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(parser.MainGroup()->Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theArgument, option->Argument());
    EXPECT_NULL(parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddOptionOptionalArgumentOptionFound)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    parser.AddOptionOptionalArgument(LongName, ShortName, Description, &optionFound, &theArgument);
    EXPECT_EQ(size_t(2), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(size_t(1), parser.MainGroup()->Options().size());
    EXPECT_EQ(ArgTypeOptional, parser.MainGroup()->Options()[0]->ArgType());
    EXPECT_EQ(LongName, parser.MainGroup()->Options()[0]->LongName());
    EXPECT_EQ(ShortName, parser.MainGroup()->Options()[0]->ShortName());
    EXPECT_EQ(Description, parser.MainGroup()->Options()[0]->Description());
    const CommandLineOptionWithArgument<int> * option =
        dynamic_cast<const CommandLineOptionWithArgument<int> *>(parser.MainGroup()->Options()[0].get());
    EXPECT_NOT_NULL(option);
    EXPECT_EQ(&theArgument, option->Argument());
    EXPECT_EQ(&optionFound, parser.MainGroup()->Options()[0]->OptionFoundFlag());
}

TEST_FIXTURE(CommandLineParserTest, AddGroup)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);
    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupName, GroupDescription));

    parser.AddGroup(group);
    EXPECT_EQ(size_t(3), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(MainGroupName, parser.Groups()[0]->Name());
    EXPECT_EQ(MainGroupDescription, parser.Groups()[0]->Description());
    EXPECT_EQ(CommandLineParser::HelpOptionsGroupName, parser.Groups()[1]->Name());
    EXPECT_EQ("", parser.Groups()[1]->Description());
    EXPECT_EQ(group, parser.Groups()[2]);
    EXPECT_EQ(GroupName, parser.Groups()[2]->Name());
    EXPECT_EQ(GroupDescription, parser.Groups()[2]->Description());
}

TEST_FIXTURE(CommandLineParserTest, GetSetMainGroup)
{
    CommandLineParser parser;
    CommandLineOptionGroupPtr group1(new CommandLineOptionGroup(GroupName, GroupDescription));
    CommandLineOptionGroupPtr group2(new CommandLineOptionGroup(MainGroupName, MainGroupDescription));

    parser.AddGroup(group1);
    parser.AddGroup(group2);
    EXPECT_EQ(size_t(4), parser.Groups().size());
    EXPECT_EQ(parser.MainGroup(), parser.Groups()[0]);
    EXPECT_EQ(CommandLineParser::DefaultMainOptionsGroupName, parser.Groups()[0]->Name());
    EXPECT_EQ("", parser.Groups()[0]->Description());
    EXPECT_EQ(CommandLineParser::HelpOptionsGroupName, parser.Groups()[1]->Name());
    EXPECT_EQ("", parser.Groups()[1]->Description());
    EXPECT_EQ(group1, parser.Groups()[2]);
    EXPECT_EQ(GroupName, parser.Groups()[2]->Name());
    EXPECT_EQ(GroupDescription, parser.Groups()[2]->Description());
    EXPECT_EQ(group2, parser.Groups()[3]);
    EXPECT_EQ(MainGroupName, parser.Groups()[3]->Name());
    EXPECT_EQ(MainGroupDescription, parser.Groups()[3]->Description());

    parser.MainGroup(group2);
    EXPECT_EQ(parser.MainGroup(), group2);
}

TEST_FIXTURE(CommandLineParserTest, GetHelp)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);
    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file");

    string expected = "Usage:\n"
                     "  GLib++.Test [OPTION...]\n\n"
                     "Help Options:\n"
                     "  -h, --help         Show help options\n\n"
                     "Application Options:\n"
                     "  -s, --runsuite     Run only specified test suite\n"
                     "  -f, --runfixture   Run only specified test fixture\n"
                     "  -t, --runtest      Run only specified test\n"
                     "  --xmloutput        Send output to specified XML file\n\n";
    string actual = parser.GetHelp("GLib++.Test");
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CommandLineParserTest, GetHelpOnGroup)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);
    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file");

    string expected = "Usage:\n"
                     "  GLib++.Test [OPTION...]\n\n"
                     "Application Options:\n"
                     "  -s, --runsuite     Run only specified test suite\n"
                     "  -f, --runfixture   Run only specified test fixture\n"
                     "  -t, --runtest      Run only specified test\n"
                     "  --xmloutput        Send output to specified XML file\n\n";
    string actual = parser.GetHelp("GLib++.Test", true, group);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(CommandLineParserTest, ParseEmpty)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    parser.Parse(argc, argv);
    EXPECT_EQ("", testSuite);
    EXPECT_EQ("", testFixture);
    EXPECT_EQ("", test);
    EXPECT_FALSE(xmlOutput);
    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;

    parser.AddGroup(group);

    group->AddOptionNoArgument("option_a", 'a', "A option", &flagA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-a");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionMultipleNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;

    parser.AddGroup(group);

    group->AddOptionNoArgument("option_a", 'a', "A option", &flagA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-abc");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_TRUE(flagB);
    EXPECT_TRUE(flagC);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionMultipleFailsOnOptionalArgumentOption)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionOptionalArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-abc");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserCannotGroupException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionMultipleFailsOnRequiredArgumentOption)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-abc");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserCannotGroupException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionRequiredArgumentFailsWhenNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-a");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserExpectedArgumentException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionRequiredArgumentSucceedsWhenArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-a");
    AddCommandLineParameter(argc, argv, "true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionOptionalArgumentSucceedsWhenNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionOptionalArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-a");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_FALSE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseShortOptionOptionalArgumentSucceedsWhenArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-a");
    AddCommandLineParameter(argc, argv, "true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;

    parser.AddGroup(group);

    group->AddOptionNoArgument("option_a", 'a', "A option", &flagA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionRequiredArgumentFailsWhenNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserExpectedArgumentException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionRequiredArgumentSucceedsWhenArgumentUsingEquals)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a=true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionRequiredArgumentSucceedsWhenArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a");
    AddCommandLineParameter(argc, argv, "true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionOptionalArgumentSucceedsWhenNoArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionOptionalArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_FALSE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionOptionalArgumentSucceedsWhenArgumentUsingEquals)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a=true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseLongOptionOptionalArgumentSucceedsWhenArgument)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    bool flagA;
    bool flagB;
    bool flagC;
    bool valueA = false;

    parser.AddGroup(group);

    group->AddOptionRequiredArgument("option_a", 'a', "A option", &flagA, &valueA);
    group->AddOptionNoArgument("option_b", 'b', "B option", &flagB);
    group->AddOptionNoArgument("option_c", 'c', "C option", &flagC);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--option_a");
    AddCommandLineParameter(argc, argv, "true");
    parser.Parse(argc, argv);

    EXPECT_TRUE(flagA);
    EXPECT_FALSE(flagB);
    EXPECT_FALSE(flagC);
    EXPECT_TRUE(valueA);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseSwitch)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--xmloutput");
    parser.Parse(argc, argv);
    EXPECT_EQ("", testSuite);
    EXPECT_EQ("", testFixture);
    EXPECT_EQ("", test);
    EXPECT_TRUE(xmlOutput);
    EXPECT_FALSE(group->Options()[0]->FoundOption());
    EXPECT_FALSE(group->Options()[1]->FoundOption());
    EXPECT_FALSE(group->Options()[2]->FoundOption());
    EXPECT_TRUE(group->Options()[3]->FoundOption());

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionLong)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--runsuite");
    AddCommandLineParameter(argc, argv, "suite");
    parser.Parse(argc, argv);
    EXPECT_EQ("suite", testSuite);
    EXPECT_EQ("", testFixture);
    EXPECT_EQ("", test);
    EXPECT_FALSE(xmlOutput);
    EXPECT_TRUE(group->Options()[0]->FoundOption());
    EXPECT_FALSE(group->Options()[1]->FoundOption());
    EXPECT_FALSE(group->Options()[2]->FoundOption());
    EXPECT_FALSE(group->Options()[3]->FoundOption());

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionLongWithEquals)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--runsuite=suite");
    parser.Parse(argc, argv);
    EXPECT_EQ("suite", testSuite);
    EXPECT_EQ("", testFixture);
    EXPECT_EQ("", test);
    EXPECT_FALSE(xmlOutput);
    EXPECT_TRUE(group->Options()[0]->FoundOption());
    EXPECT_FALSE(group->Options()[1]->FoundOption());
    EXPECT_FALSE(group->Options()[2]->FoundOption());
    EXPECT_FALSE(group->Options()[3]->FoundOption());

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionShort)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-s");
    AddCommandLineParameter(argc, argv, "suite");
    parser.Parse(argc, argv);
    EXPECT_EQ("suite", testSuite);
    EXPECT_EQ("", testFixture);
    EXPECT_EQ("", test);
    EXPECT_FALSE(xmlOutput);
    EXPECT_TRUE(group->Options()[0]->FoundOption());
    EXPECT_FALSE(group->Options()[1]->FoundOption());
    EXPECT_FALSE(group->Options()[2]->FoundOption());
    EXPECT_FALSE(group->Options()[3]->FoundOption());

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionNoArgumentExpectedFailure)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    parser.AddGroup(group);
    group->AddOptionNoArgument("anoption", 'a', "An option");

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--anoption=true");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserUnexpectedArgumentException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionRequiredArgumentExpectedFailure)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    parser.AddGroup(group);
    group->AddOptionRequiredArgument<int>("anoption", 'a', "An option", nullptr);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "--anoption");
    EXPECT_THROW(parser.Parse(argc, argv), CommandLineParserExpectedArgumentException);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseWithNonOptions)
{
    CommandLineParser parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    parser.AddGroup(group);
    group->AddOptionRequiredArgument<int>("anoption", 'a', "An option", nullptr);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "parameter");
    parser.Parse(argc, argv);
    ASSERT_EQ(1, parser.NumNonOptions());
    EXPECT_EQ("parameter", parser.NonOptions()[0]);

    DestroyCommandLineParameterList(argc, argv);
}

TEST_FIXTURE(CommandLineParserTest, ParseOptionCallback)
{
    CommandLineParserWithCallback parser(MainGroupName, MainGroupDescription);

    CommandLineOptionGroupPtr group(new CommandLineOptionGroup(GroupNameApplications, GroupDescriptionApplications));

    string testSuite;
    string testFixture;
    string test;
    bool xmlOutput = false;
    parser.AddGroup(group);
    group->AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite", &testSuite);
    group->AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture", &testFixture);
    group->AddOptionRequiredArgument("runtest", 't', "Run only specified test", &test);
    group->AddOptionNoArgument("xmloutput", 0, "Send output to specified XML file", &xmlOutput);

    int argc = 0;
    char * const * argv = nullptr;
    AddCommandLineParameter(argc, argv, "GLib++.Test");
    AddCommandLineParameter(argc, argv, "-s");
    AddCommandLineParameter(argc, argv, "suite");
    AddCommandLineParameter(argc, argv, "--runfixture");
    AddCommandLineParameter(argc, argv, "fixture");
    AddCommandLineParameter(argc, argv, "--runtest=test");
    AddCommandLineParameter(argc, argv, "--xmloutput");
    parser.Parse(argc, argv);
    EXPECT_EQ("suite", testSuite);
    EXPECT_EQ("fixture", testFixture);
    EXPECT_EQ("test", test);
    EXPECT_TRUE(xmlOutput);
    EXPECT_TRUE(group->Options()[0]->FoundOption());
    EXPECT_TRUE(group->Options()[1]->FoundOption());
    EXPECT_TRUE(group->Options()[2]->FoundOption());
    EXPECT_TRUE(group->Options()[3]->FoundOption());

    EXPECT_EQ("suite", parser.runSuite);
    EXPECT_EQ("fixture", parser.runFixture);
    EXPECT_EQ("test", parser.runTest);
    EXPECT_TRUE(parser.xmlOutput);

    DestroyCommandLineParameterList(argc, argv);
}

} // namespace Test

} // namespace Core
