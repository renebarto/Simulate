#include "unit-test-c++/UnitTestC++.h"
#include "core/Exception.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class ExceptionTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ExceptionTest::SetUp()
{
}

void ExceptionTest::TearDown()
{
}

static char const * Location = "Location";
static char const * FileName = "abc.cpp";
static const int Line = 123;
static const std::string Message = "Some message";
static const std::string Message2 = "Some other message";
static const std::string Argument = "Argument";

TEST_FIXTURE(ExceptionTest, BaseExceptionConstructDefault)
{
    BaseException e;

    std::ostringstream stream;
    stream << "Core::BaseException";
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, BaseExceptionConstructInnerException)
{
    Exception inner(Location, FileName, Line, Message);
    BaseException e(inner);

    std::ostringstream stream;
    stream << "Core::BaseException" << std::endl << "-->" << inner.what();
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, BaseExceptionConstructMessage)
{
    BaseException e(Message);

    std::ostringstream stream;
    stream << "Core::BaseException: " << Message;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, BaseExceptionConstructMessageInnerException)
{
    Exception inner(Location, FileName, Line, Message);
    BaseException e(Message2, inner);

    std::ostringstream stream;
    stream << "Core::BaseException: " << Message2 << std::endl << "-->" << inner.what();
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, BaseExceptionBuildMessage)
{
    BaseException e;

    EXPECT_EQ("", e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, BaseExceptionGetSetMessage)
{
    BaseException e;

    EXPECT_EQ("", e.GetMessage());
    e.SetMessage(Message);
    EXPECT_EQ(Message, e.GetMessage());
}

TEST_FIXTURE(ExceptionTest, ExceptionConstructFunctionFileLine)
{
    Exception e(Location, FileName, Line);

    std::ostringstream stream;
    stream << "Core::Exception - in " << Location << " [" << FileName << ":" << Line << "]";
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ExceptionConstructFunctionFileLineMessage)
{
    Exception e(Location, FileName, Line, Message2);

    std::ostringstream stream;
    stream << "Core::Exception - in " << Location << " [" << FileName << ":" << Line << "]" << ": " << Message2;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ExceptionConstructFunctionFileLineMessageInnerException)
{
    Exception inner(Location, FileName, Line, Message);
    Exception e(Location, FileName, Line, Message2, inner);

    std::ostringstream stream;
    stream << "Core::Exception - in " << Location << " [" << FileName << ":" << Line << "]" << ": " << Message2 << std::endl
           << "-->" << inner.what();
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ExceptionBuildMessage)
{
    Exception e(Location, FileName, Line);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]";
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, ArgumentExceptionConstructFunctionFileLineArgument)
{
    ArgumentException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "Core::ArgumentException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument: " << Argument;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ArgumentExceptionConstructFunctionFileLineArgumentMessage)
{
    ArgumentException e(Location, FileName, Line, Argument, Message);

    std::ostringstream stream;
    stream << "Core::ArgumentException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument: " << Argument << ": " << Message;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ArgumentExceptionBuildMessage)
{
    ArgumentException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument: " << Argument;
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, ArgumentOutOfRangeExceptionConstructFunctionFileLineArgument)
{
    ArgumentOutOfRangeException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "Core::ArgumentOutOfRangeException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument out of range: " << Argument;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ArgumentOutOfRangeExceptionConstructFunctionFileLineArgumentMessage)
{
    ArgumentOutOfRangeException e(Location, FileName, Line, Argument, Message);

    std::ostringstream stream;
    stream << "Core::ArgumentOutOfRangeException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument out of range: " << Argument << ": " << Message;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ArgumentOutOfRangeExceptionBuildMessage)
{
    ArgumentOutOfRangeException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument out of range: " << Argument;
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, ArgumentNullExceptionConstructFunctionFileLineArgument)
{
    ArgumentNullException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "Core::ArgumentNullException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument null: " << Argument;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, ArgumentNullExceptionBuildMessage)
{
    ArgumentNullException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]"
           << ": argument null: " << Argument;
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, OperationExceptionConstructFunctionFileLineLocationMessage)
{
    OperationException e(Location, FileName, Line, Message);

    std::ostringstream stream;
    stream << "Core::OperationException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": " << Message;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, OperationExceptionBuildMessage)
{
    OperationException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]";
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

TEST_FIXTURE(ExceptionTest, OperationNotSupportedExceptionConstructFunctionFileLineLocationMessage)
{
    OperationNotSupportedException e(Location, FileName, Line, Message);

    std::ostringstream stream;
    stream << "Core::OperationNotSupportedException - in " << Location << " [" << FileName << ":" << Line << "]"
           << ": operation not supported: " << Message;
    EXPECT_EQ(stream.str(), e.what());
}

TEST_FIXTURE(ExceptionTest, OperationNotSupportedExceptionBuildMessage)
{
    OperationNotSupportedException e(Location, FileName, Line, Argument);

    std::ostringstream stream;
    stream << "in " << Location << " [" << FileName << ":" << Line << "]"
           << ": operation not supported";
    EXPECT_EQ(stream.str(), e.BuildMessage());
}

} // namespace Test

} // namespace Core
