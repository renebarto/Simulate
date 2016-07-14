#include "core/Util.h"

#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstring>
#include "osal.h"
#include "core/ByteArray.h"
#include "core/DefaultLogger.h"
#include "core/String.h"

using namespace std;
using namespace Core;

timespec Util::ConvertToTimeSpec(int ms)
{
    timespec time;
    time.tv_sec = ms / 1000;
    time.tv_nsec = (ms % 1000) * 1000000;
    return time;
}

timespec Util::ConvertTimeOffsetToTimeSpec(int offsetMS)
{
    timespec time;
    OSAL::clock_gettime(CLOCK_REALTIME, &time);
    time.tv_nsec += (offsetMS % 1000) * 1000000;
    time.tv_sec += offsetMS / 1000;
    time.tv_sec += time.tv_nsec / 1000000000;
    time.tv_nsec %= 1000000000;
    return time;
}

timespec Util::ConvertDurationToTimeSpec(std::chrono::milliseconds duration)
{
    timespec time;
    time.tv_sec = duration.count() / 1000;
    time.tv_nsec = (duration.count() % 1000) * 1000000;
    return time;
}

timespec Util::ConvertDurationToTimeSpec(std::chrono::microseconds duration)
{
    timespec time;
    time.tv_sec = duration.count() / 1000000;
    time.tv_nsec = (duration.count() % 1000000) * 1000;
    return time;
}

timespec Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds duration)
{
    timespec time;
    time.tv_sec = duration.count() / 1000000000;
    time.tv_nsec = duration.count() % 1000000000;
    return time;
}

void Util::Sleep(int sleepMS)
{
    timespec sleepTime = ConvertToTimeSpec(sleepMS);
    int errorCode;
    do
    {
        errorCode = OSAL::nanosleep(&sleepTime, &sleepTime);
        if (errorCode == -1)
            errorCode = errno;
    }
    while (errorCode == EINTR);
    Util::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
}

int Util::NumDigits(int64_t n) {
    return (n != 0) ? static_cast<int>(std::log10(std::abs(static_cast<double>(n)))) + 1 : 1;
}

void Util::ZeroMem(void * destination, int size)
{
	ZeroMem(destination, 0, size);
}
void Util::ZeroMem(void * destination, int offset, int size)
{
    memset((uint8_t *)destination + offset, 0, size);
}

void Util::CopyMem(void * destination, const void * source, int size)
{
	CopyMem(destination, 0, source, 0, size);
}
void Util::CopyMem(void * destination, int offsetDestination, const void * source, int size)
{
	CopyMem(destination, offsetDestination, source, 0, size);
}
void Util::CopyMem(void * destination, const void * source, int offsetSource, int size)
{
	CopyMem(destination, 0, source, offsetSource, size);
}
void Util::CopyMem(void * destination, int offsetDestination, const void * source,
                   int offsetSource, int size)
{
    memcpy((uint8_t *)destination + offsetDestination, (uint8_t *)source + offsetSource, size);
}

void Util::DumpToStream(std::ostream & stream, const ByteArray & data)
{
    DumpToStream(stream, data.Data(), data.Size());
}

static const size_t BytesPerRow = 16;

void Util::DumpToStream(ostream & stream, const uint8_t * data, size_t length)
{
    for (size_t offset = 0; offset < length; offset += BytesPerRow)
    {
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < length)
            {
                uint8_t value = data[i + offset];
                stream << hex << setw(2) << setfill('0') << (int)value << " ";
            }
            else
            {
                stream << "   ";
            }
        }
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < length)
            {
                uint8_t value = data[i + offset];
                stream << (char)(((value >= 32) && (value < 128)) ? value : '?') << " ";
            }
        }
        stream << endl;
    }
    stream << endl << flush;
}

static const unsigned char charToBase64[256] =
{
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,  0,255,255,
    255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
    255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

static const char base64ToChar[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Util::Base64Encode(const ByteArray & data)
{
    string result;
    size_t index = 0;
	size_t size = data.Size();
    while (index < size - 2)
    {
        result += base64ToChar[(data[index + 0] & 0xFC) >> 2];
        result += base64ToChar[(data[index + 0] & 0x03) << 4 | (data[index + 1] & 0xF0) >> 4];
        result += base64ToChar[(data[index + 1] & 0x0F) << 2 | (data[index + 2] & 0xC0) >> 6];
        result += base64ToChar[(data[index + 2] & 0x3F) >> 0];
        index += 3;
    }
    if ((size % 3) == 1)
    {
        result += base64ToChar[(data[index + 0] & 0xFC) >> 2];
        result += base64ToChar[(data[index + 0] & 0x03) << 4];
        result += "==";
    }
    if ((size % 3) == 2)
    {
        result += base64ToChar[(data[index + 0] & 0xFC) >> 2];
        result += base64ToChar[(data[index + 0] & 0x03) << 4 | (data[index + 1] & 0xF0) >> 4];
        result += base64ToChar[(data[index + 1] & 0x0F) << 2];
        result += '=';
    }
    return result;
}

ByteArray Util::Base64Decode(const std::string & dataBase64)
{
    return Base64Decode(dataBase64.c_str());
}

ByteArray Util::Base64Decode(const char * dataBase64)
{
    ByteArray result;
    if (dataBase64 == nullptr)
        throw Core::ArgumentNullException(__func__, __FILE__, __LINE__, "dataBase64");
	size_t index = 0;
	size_t offset = 0;
	size_t length = strlen(dataBase64);
    while (index < length)
    {
        uint8_t base64_0 {};
        uint8_t base64_1 {};
        uint8_t base64_2 {};
        uint8_t base64_3 {};
        bool endCharFound {};

        base64_0 = charToBase64[size_t(dataBase64[index])];
        if (base64_0 == 0xFF)
            throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Incorrect format for base64 string at offset " +
                                          Util::ToString(index) +
                                          ": " + string(dataBase64));
        index++;

        if (index >= length)
            throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                "Incorrect format for base64 string, expected more input at offset " +
                Util::ToString(index) + ": " + string(dataBase64));
        base64_1 = charToBase64[size_t(dataBase64[index])];
        if (base64_1 == 0xFF)
            throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Incorrect format for base64 string at offset " +
                                          Util::ToString(index) +
                                          ": " + string(dataBase64));
        result.SetUInt8(offset++, (base64_0 & 0x3F) << 2 | (base64_1 & 0x30) >> 4);
        index++;

        if ((index < length) & (dataBase64[index] == '='))
        {
            endCharFound = true;
        }
        if ((index < length) & !endCharFound)
        {
            base64_2 = charToBase64[size_t(dataBase64[index])];
            if (base64_2 == 0xFF)
                throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                              "Incorrect format for base64 string at offset " +
                                              Util::ToString(index) +
                                              ": " + string(dataBase64));
            result.SetUInt8(offset++, (base64_1 & 0x0F) << 4 | (base64_2 & 0x3C) >> 2);
        }
        index++;

        if ((index < length) & endCharFound && (dataBase64[index] != '='))
        {
            throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                          "Expected end of string or '=' for base64 string at offset " +
                                          Util::ToString(index) +
                                          ": " + string(dataBase64));
        }
        if ((index < length) & (dataBase64[index] == '='))
        {
            endCharFound = true;
        }
        if ((index < length) & !endCharFound)
        {
            base64_3 = charToBase64[size_t(dataBase64[index])];
            if (base64_3 == 0xFF)
                throw Core::ArgumentException(__func__, __FILE__, __LINE__, "dataBase64",
                                              "Incorrect format for base64 string at offset " +
                                              Util::ToString(index) +
                                              ": " + string(dataBase64));
            result.SetUInt8(offset++, (base64_2 & 0x03) << 6 | (base64_3 & 0x3F) >> 0);
        }
        index++;
    }
    return result;
}

bool HasValidCharactersForBase(const string & text, int base)
{
    if (text.empty())
        return false;
    switch (base)
    {
    case 2:
        for (size_t i = 0; i < text.length(); i++)
        {
            if (strchr("01", toupper(text[i])) == nullptr)
                return false;
        }
        break;
    case 8:
        for (size_t i = 0; i < text.length(); i++)
        {
            if (strchr("01234567", toupper(text[i])) == nullptr)
                return false;
        }
        break;
    case 10:
        for (size_t i = 0; i < text.length(); i++)
        {
            if (strchr("0123456789+-", toupper(text[i])) == nullptr)
                return false;
        }
        break;
    case 16:
        for (size_t i = 0; i < text.length(); i++)
        {
            if (strchr("0123456789ABCDEF", toupper(text[i])) == nullptr)
                return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

bool HasValidCharactersFloatingPoint(const string & text)
{
    if (text.empty())
        return false;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (strchr("0123456789.+-Ee", toupper(text[i])) == nullptr)
            return false;
    }
    return true;
}

bool Util::TryParse(const string & text, int8_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < CHAR_MIN) || (result > CHAR_MAX)))
        return false;
    value = (int8_t)result;

    return true;
}

bool Util::TryParse(const string & text, bool & value)
{
    if (Core::String::IsEqualIgnoreCase("true", text))
    {
        value = true;
        return true;
    }
    if (Core::String::IsEqualIgnoreCase("false", text))
    {
        value = false;
        return true;
    }

    return false;
}

bool Util::TryParse(const string & text, uint8_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > UCHAR_MAX)))
        return false;
    value = (uint8_t)result;

    return true;
}

bool Util::TryParse(const string & text, int16_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < SHRT_MIN) || (result > SHRT_MAX)))
        return false;
    value = (int16_t)result;

    return true;
}

bool Util::TryParse(const string & text, uint16_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long result = strtol(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < 0) || (result > USHRT_MAX)))
        return false;
    value = (uint16_t)result;

    return true;
}

bool Util::TryParse(const string & text, int32_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long long result = strtoll(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < INT_MIN) || (result > INT_MAX)))
        return false;
    value = (int32_t)result;

    return true;
}

bool Util::TryParse(const string & text, uint32_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    unsigned long long result = strtoull(text.c_str(), nullptr, base);
    if ((base == 10) && (result > (unsigned long)UINT_MAX))
        return false;
    value = (uint32_t)result;

    return true;
}

bool Util::TryParse(const string & text, int64_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    long long result = strtoull(text.c_str(), nullptr, base);
    if ((base == 10) && ((result < LLONG_MIN) || (result > LLONG_MAX)))
        return false;
    value = (int64_t)result;

    return true;
}

bool Util::TryParse(const string & text, uint64_t & value, int base /*= 10*/)
{
    if (!HasValidCharactersForBase(text, base))
        return false;

    unsigned long long result = strtoull(text.c_str(), nullptr, base);
    value = (uint64_t)result;

    return true;
}

bool Util::TryParse(const string & text, float & value)
{
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    istringstream stream(text);
    stream >> value;

    return true;
}

bool Util::TryParse(const string & text, double & value)
{
    if (!HasValidCharactersFloatingPoint(text))
        return false;

    istringstream stream(text);
    stream >> value;

    return true;
}

string Util::ToString(bool value)
{
    return value ? "true" : "false";
}

string Util::ToString(uint8_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<8> x(value);
        stream << setfill('0') << setw(8) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(3) << (int)value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << (int)value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(2) << (int)value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(int16_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<16> x(value);
        stream << setfill('0') << setw(16) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(6) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(4) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(uint16_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<16> x(value);
        stream << setfill('0') << setw(16) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(6) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(4) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(int32_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<32> x(value);
        stream << setfill('0') << setw(32) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(11) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(8) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(uint32_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<32> x(value);
        stream << setfill('0') << setw(32) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(11) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(8) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(int64_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<64> x(value);
        stream << setfill('0') << setw(64) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(22) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(16) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(uint64_t value, int base)
{
    ostringstream stream;

    switch (base)
    {
    case 2:
    {
        std::bitset<64> x(value);
        stream << setfill('0') << setw(64) << x;
    }
    break;
    case 8:
    {
        stream << oct << setfill('0') << setw(22) << value;
    }
    break;
    case 10:
    {
        stream << dec << setfill('0') << value;
    }
    break;
    case 16:
    {
        stream << hex << uppercase << setfill('0') << setw(16) << value;
    }
    break;
    default:
        throw Core::ArgumentException(__func__, __FILE__, __LINE__, "base",
                                      "Invalid base specified, only 2, 8, 10, 16 supported");
    }
    return stream.str();
}

string Util::ToString(float value, int precision)
{
    std::ostringstream stream;

    stream << setprecision(precision) << value;

    return stream.str();
}

string Util::ToString(double value, int precision)
{
    std::ostringstream stream;

    stream << setprecision(precision) << value;

    return stream.str();
}

string Util::ToString(string value, bool quote)
{
    std::ostringstream stream;

    if (quote)
        stream << "\"";
    stream << value;
    if (quote)
        stream << "\"";

    return stream.str();
}

class FileWrapper
{
public:
    FileWrapper() :
        handle(nullptr)
    {
    }
    FileWrapper(const FileWrapper &) = delete;
    ~FileWrapper()
    {
        Close();
    }

    FileWrapper & operator = (const FileWrapper &) = delete;

    operator FILE * ()
    {
        return handle;
    }
    bool Open(const char * path, const char * mode)
    {
        Close();
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
		handle = fopen(path, mode);
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
		return (handle != nullptr);
    }
    void Close()
    {
        if (handle != nullptr)
            fclose(handle);
        handle = nullptr;
    }
    size_t Size()
    {
        fseek(handle, 0, SEEK_END);
        size_t result = ftell(handle);
        fseek(handle, 0, SEEK_SET);
        return result;
    }
    size_t Read(uint8_t * buffer, size_t numBytes)
    {
        return fread(buffer, sizeof(uint8_t), numBytes, handle);
    }
    size_t Write(const uint8_t * buffer, size_t numBytes)
    {
        return fwrite(buffer, sizeof(uint8_t), numBytes, handle);
    }

protected:
    FILE * handle;
};

class MallocBuffer
{
public:
    MallocBuffer() = delete;
    MallocBuffer(const MallocBuffer &) = delete;
    MallocBuffer(size_t size) :
        size(size),
        contents(nullptr)
    {
        contents = (char *)malloc(size);
    }
    ~MallocBuffer()
    {
        free(contents);
        size = 0;
        contents = nullptr;
    }

    MallocBuffer & operator = (const MallocBuffer &) = delete;

    size_t size;
    char * contents;
};

ssize_t ReadLine(MallocBuffer & buffer, FILE * file)
{
    ssize_t count = OSAL::getline(&buffer.contents, &buffer.size, file);
    return count;
}

bool Util::CompareFiles(const char* pathA, const char* pathB)
{
    bool result = false;
    FileWrapper fileA;
    FileWrapper fileB;

    if (fileA.Open(pathA, "rb"))
    {
        if (fileB.Open(pathB, "rb"))
        {
            size_t fileSizeA = fileA.Size();
			size_t fileSizeB = fileB.Size();
            if ((fileSizeA != fileSizeB))
                return false;
            ByteArray bufferA(fileSizeA);
            ByteArray bufferB(fileSizeB);
            size_t bytesReadA = fileA.Read(bufferA, fileSizeA);
            size_t bytesReadB = fileB.Read(bufferB, fileSizeB);
            if (bytesReadA != (size_t)fileSizeA)
                return false;
            if (bytesReadB != (size_t)fileSizeB)
                return false;
           result = Compare(bufferA.Data(), bufferB.Data(), fileSizeA);
        }
    }
    return result;
}

bool Util::CompareFiles(const string & pathA, const string & pathB)
{
    return CompareFiles(pathA.c_str(), pathB.c_str());
}

bool CompareLines(MallocBuffer & bufferA, FileWrapper & fileA,
                  MallocBuffer & bufferB, FileWrapper & fileB)
{
    int line = 1;
    while (!feof(fileA) && !feof(fileB))
    {
        ssize_t countA = ReadLine(bufferA, fileA);
        ssize_t countB = ReadLine(bufferB, fileB);
        if ((countA == -1) && (countB == -1))
            return true;
        if ((countA == -1) || (countB == -1))
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        if (countA != countB)
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        if (strcmp(bufferA.contents, bufferB.contents) != 0)
        {
            ostringstream stream;
            stream << "Difference between text files in line " << line;
            TheLogger().Warn("Util::CompareTextFiles", stream.str());
            return false;
        }
        line++;
    }
    return (feof(fileA) && feof(fileB));
}

bool Util::CompareTextFiles(const char* pathA, const char* pathB)
{
    bool result = false;
    FileWrapper fileA;
    FileWrapper fileB;

    if (fileA.Open(pathA, "r"))
    {
        if (fileB.Open(pathB, "r"))
        {
            const size_t BufferSize = 1024;
            MallocBuffer bufferA(BufferSize);
            MallocBuffer bufferB(BufferSize);
            result = CompareLines(bufferA, fileA, bufferB, fileB);
        }
    }
    return result;
}

bool Util::CompareTextFiles(const string & pathA, const string & pathB)
{
    return CompareTextFiles(pathA.c_str(), pathB.c_str());
}

bool CompareLogLines(MallocBuffer & bufferA, FileWrapper & fileA,
                     MallocBuffer & bufferB, FileWrapper & fileB)
{
    while (!feof(fileA) && !feof(fileB))
    {
        ssize_t countA = ReadLine(bufferA, fileA);
        ssize_t countB = ReadLine(bufferB, fileB);
        if ((countA == -1) || (countB == -1))
            break;
        if (countA != countB)
        {
            return false;
        }
        string stringA = string(bufferA.contents, countA);
        string stringB = string(bufferB.contents, countB);
        stringA = stringA.substr(stringA.find(" - "));
        stringB = stringB.substr(stringB.find(" - "));
        if (stringA != stringB)
            return false;
    }
    return (feof(fileA) && feof(fileB));
}

bool Util::CompareLogFiles(const char* pathA, const char* pathB)
{
    bool result = false;
    FileWrapper fileA;
    FileWrapper fileB;

    if (fileA.Open(pathA, "r"))
    {
        if (fileB.Open(pathB, "r"))
        {
            const size_t BufferSize = 1024;
            MallocBuffer bufferA(BufferSize);
            MallocBuffer bufferB(BufferSize);
            result = CompareLogLines(bufferA, fileA, bufferB, fileB);
        }
    }
    return result;
}

bool Util::CompareLogFiles(const string & pathA, const string & pathB)
{
    return CompareLogFiles(pathA.c_str(), pathB.c_str());
}

bool Util::Compare(double expected, double actual, double epsilonAbs, double epsilonRel)
{
    if (std::isinf(expected) && std::isinf(actual))
    {
        return (std::isinf(expected) == std::isinf(actual));
    }
    double errorAbs = fabs(expected - actual);
    double deltaMax = std::max(epsilonAbs, epsilonRel * std::max(fabs(actual), fabs(expected)));
    return (errorAbs <= deltaMax);
}
bool Util::Compare(float expected, float actual, float epsilonAbs, float epsilonRel)
{
    if (std::isinf(expected) && std::isinf(actual))
    {
        return (std::isinf(expected) == std::isinf(actual));
    }
    double errorAbs = fabsf(expected - actual);
    double deltaMax = std::max(epsilonAbs, epsilonRel * std::max(fabsf(actual), fabsf(expected)));
    return (errorAbs <= deltaMax);
}

void Util::ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode)
{
    if (errorCode != 0)
    {
        throw SystemError(functionName, fileName, line, errorCode);
    }
}
