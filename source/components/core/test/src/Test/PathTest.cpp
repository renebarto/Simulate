#include "unit-test-c++/UnitTestC++.h"

#include "core/ByteArray.h"
#include "core/Path.h"
#include "core/TestData.h"

using namespace std;

namespace Core
{

    namespace Test
    {

        class PathTest : public UnitTestCpp::TestFixture
        {
        public:
            virtual void SetUp();
            virtual void TearDown();
        };

        void PathTest::SetUp()
        {
        }

        void PathTest::TearDown()
        {
        }

        TEST_FIXTURE(PathTest, FileExists)
        {
            EXPECT_TRUE(Path::FileExists(TestData::RegularFilePath()));
            EXPECT_FALSE(Path::FileExists(TestData::FilledDirPath()));
            EXPECT_FALSE(Path::FileExists(TestData::DummyFilePath()));
        }

        TEST_FIXTURE(PathTest, DirectoryExists)
        {
            EXPECT_FALSE(Path::DirectoryExists(TestData::RegularFilePath()));
            EXPECT_TRUE(Path::DirectoryExists(TestData::FilledDirPath()));
            EXPECT_FALSE(Path::DirectoryExists(TestData::DummyDirPath()));
        }

#if defined(__linux__)
        TEST_FIXTURE(PathTest, ResolveTilde)
        {
            EXPECT_EQ(".", Path::ResolveTilde("."));
            EXPECT_EQ(TestData::FilledDirPath(), Path::ResolveTilde(TestData::FilledDirPath()));
            string home = OSAL::getenv("HOME");
            string expected = Path::CombinePath(home, "data");
            EXPECT_EQ(expected, Path::ResolveTilde("~/data"));
        }
#endif

        TEST_FIXTURE(PathTest, SplitPath)
        {
            string path = TestData::RegularFilePath();
            string directoryActual;
            string fileNameActual;
            string directoryExpected = TestData::FilledDirPath();
            string fileNameExpected = TestData::RegularFileName();

            Path::SplitPath(path, directoryActual, fileNameActual);
            EXPECT_EQ(directoryExpected, directoryActual);
            EXPECT_EQ(fileNameExpected, fileNameActual);
        }

        TEST_FIXTURE(PathTest, Extension)
        {
            string path = TestData::RegularFilePath();
            string expected = TestData::RegularFileExtension();

            EXPECT_EQ(expected, Path::Extension(path));
        }

        TEST_FIXTURE(PathTest, StripExtension)
        {
            string path = TestData::RegularFilePath();
            string expected = Path::CombinePath(TestData::FilledDirPath(), TestData::RegularFileNameNoExtension());

            EXPECT_EQ(expected, Path::StripExtension(path));
        }

        TEST_FIXTURE(PathTest, HasNoPath)
        {
            EXPECT_FALSE(Path::HasNoPath(TestData::RegularFilePath()));
            EXPECT_TRUE(Path::HasNoPath(TestData::RegularFileName()));
        }

        TEST_FIXTURE(PathTest, CreatePathWithDefault)
        {
            EXPECT_EQ(TestData::RegularFilePath(), Path::CreatePathWithDefault(TestData::RegularFilePath(), TestData::FilledDirPath()));
            EXPECT_EQ(TestData::RegularFilePath(), Path::CreatePathWithDefault(TestData::RegularFileName(), TestData::FilledDirPath()));
        }

        TEST_FIXTURE(PathTest, CombinePath)
        {
            string basePath = Path::InsertPathSeparator("", Path::InsertPathSeparator("Home", "User"));
            string subDirectory = Path::InsertPathSeparator("", "Subdirectory");
            string expected = Path::InsertPathSeparator(basePath, subDirectory);

            EXPECT_EQ(expected, Path::CombinePath(basePath, subDirectory));
        }

        TEST_FIXTURE(PathTest, FullPath)
        {
            string currentDirectory = Path::CurrentDir();
            string subDirectory = "testdata";
            string relativePath = Path::CombinePath("..", Path::CombinePath("..", Path::CombinePath("..", Path::CombinePath("..", subDirectory))));
            string result = currentDirectory;
            size_t pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos);
            pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos);
            pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos) + subDirectory;
            pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos + 1) + subDirectory;
            string expected = result;
            EXPECT_EQ(expected, Path::FullPath(relativePath));
        }

#if defined(__linux__)
        TEST_FIXTURE(PathTest, FullPathHome)
        {
            string home = OSAL::getenv("HOME");
            EXPECT_EQ(home, Path::FullPath("~/"));
        }
#endif

        TEST_FIXTURE(PathTest, StripPathToSubDirectory)
        {
            string currentDirectory = Path::CurrentDir();
            string stripToDirectory = "source";
            string result = currentDirectory;
            size_t pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos);
            pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos);
            pathDelimiterPos = result.find_last_of(Path::PathSeparator());
            if (pathDelimiterPos != string::npos)
                result = result.substr(0, pathDelimiterPos);
            string expected = result;
            EXPECT_EQ(expected, Path::StripPathToSubDirectory(currentDirectory, stripToDirectory));
        }

        TEST_FIXTURE(PathTest, RelativePath1)
        {
            string currentDirectory = Path::CurrentDir();
            string baseDirectory = Path::CombinePath(Path::StripPathToSubDirectory(currentDirectory, "source"), "..");
            string subDirectory = "testdata";
            string expected = Path::CombinePath("..", Path::CombinePath("..", Path::CombinePath("..", Path::CombinePath("..", subDirectory))));
            string fullPath = Path::CombinePath(baseDirectory, subDirectory);
            EXPECT_EQ(expected, Path::RelativePath(fullPath));
        }

        TEST_FIXTURE(PathTest, RelativePath2)
        {
            string currentDirectory = Path::CurrentDir();
            string baseDirectory = Path::CombinePath(Path::StripPathToSubDirectory(currentDirectory, "source"), "..");
            string subDirectory = "testdata";
            string expected = Path::CombinePath(".", subDirectory);
            string fullPath = Path::CombinePath(baseDirectory, subDirectory);
            EXPECT_EQ(0, OSAL::chdir(baseDirectory.c_str()));
            string actual = Path::RelativePath(fullPath);
            EXPECT_EQ(0, OSAL::chdir(currentDirectory.c_str()));
            EXPECT_EQ(expected, actual);
        }

        TEST_FIXTURE(PathTest, RelativePath3)
        {
            string currentDirectory = Path::CurrentDir();
            string baseDirectory = Path::StripPathToSubDirectory(currentDirectory, "source");
            string expected = Path::CombinePath("..", Path::CombinePath("..", ".."));
            string fullPath = baseDirectory;
            EXPECT_EQ(expected, Path::RelativePath(fullPath));
        }

        TEST_FIXTURE(PathTest, RelativePath4)
        {
#if defined(__linux__)
            string fullPath = "/usr/lib";
#elif defined(_WIN32)
            string fullPath = OSAL::getenv("TEMP");
#endif
            string expected = fullPath;
            EXPECT_EQ(expected, Path::RelativePath(fullPath));
        }

        TEST_FIXTURE(PathTest, CurrentDir)
        {
            string currentDirectory = Path::CurrentDir();
#if defined(__linux__)
            string newPath = "/usr/lib";
#elif defined(_WIN32)
            string newPath = OSAL::getenv("TEMP");
#endif
            EXPECT_EQ(0, OSAL::chdir(newPath.c_str()));
            string newCurrentDirectory = Path::CurrentDir();
            EXPECT_EQ(0, OSAL::chdir(currentDirectory.c_str()));
            EXPECT_EQ(newPath, newCurrentDirectory);
        }

    } // namespace Test

} // namespace Core
