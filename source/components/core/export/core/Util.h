#pragma once

#include <chrono>
#include <sstream>
#include "osal.h"
#include "core/Nullable.h"

namespace Core
{

// forward declarations
class ByteArray;

namespace Util
{

template<class T> T BitsToBytes(T numBits) {
    return (numBits + 7) / 8;
}
template<class T> T Clip(T value, T min, T max)
{
    return std::min(max, std::max(min, value));
}
timespec ConvertToTimeSpec(int ms);
timespec ConvertTimeOffsetToTimeSpec(int offsetMS);
timespec ConvertDurationToTimeSpec(std::chrono::milliseconds duration);
timespec ConvertDurationToTimeSpec(std::chrono::microseconds duration);
timespec ConvertDurationToTimeSpec(std::chrono::nanoseconds duration);
void Sleep(int sleepMS);

int NumDigits(int64_t);

void ZeroMem(void * destination, int size);
void ZeroMem(void * destination, int offset, int size);
void CopyMem(void * destination, const void * source, int size);
void CopyMem(void * destination, int offsetDestination, const void * source, int size);
void CopyMem(void * destination, const void * source, int offsetSource, int size);
void CopyMem(void * destination, int offsetDestination, const void * source, int offsetSource, int size);

template <typename T>
T NextPowerOfTwo(T value)
{
	T rest = value;
	int numberOfOnes = 0;
	T power = 0;
	while (rest != 0)
	{
		power++;
		if ((rest & T{ 0x01 }) != 0)
			numberOfOnes++;
		rest >>= 1;
	}
	return (numberOfOnes != 1) ? 1 << power : 1 << (power - 1);
}

void DumpToStream(std::ostream & stream, const ByteArray & data);
void DumpToStream(std::ostream & stream, const uint8_t * data, size_t length);

std::string Base64Encode(const ByteArray & data);
ByteArray Base64Decode(const std::string & dataBase64);
ByteArray Base64Decode(const char * dataBase64);

bool TryParse(const std::string & text, bool & value);
bool TryParse(const std::string & text, int8_t & value, int base = 10);
bool TryParse(const std::string & text, uint8_t & value, int base = 10);
bool TryParse(const std::string & text, int16_t & value, int base = 10);
bool TryParse(const std::string & text, uint16_t & value, int base = 10);
bool TryParse(const std::string & text, int32_t & value, int base = 10);
bool TryParse(const std::string & text, uint32_t & value, int base = 10);
bool TryParse(const std::string & text, int64_t & value, int base = 10);
bool TryParse(const std::string & text, uint64_t & value, int base = 10);
bool TryParse(const std::string & text, float & value);
bool TryParse(const std::string & text, double & value);
inline bool TryParse(const std::string & text, std::string & value)
{
    value = text;
    return true;
}
template <class T>
bool TryParse(const std::string & text, Nullable<T> & value)
{
    T parsedValue;
    if (TryParse(text, parsedValue))
    {
        value = parsedValue;
        return true;
    }
    value.SetNull();
    return false;
}std::string ToString(bool value);
std::string ToString(uint8_t value, int base = 10);
std::string ToString(int16_t value, int base = 10);
std::string ToString(uint16_t value, int base = 10);
std::string ToString(int32_t value, int base = 10);
std::string ToString(uint32_t value, int base = 10);
std::string ToString(int64_t value, int base = 10);
std::string ToString(uint64_t value, int base = 10);
std::string ToString(float value, int precision = 16);
std::string ToString(double value, int precision = 16);
std::string ToString(std::string value, bool quote = true);
template <class T> std::string ToString(T value)
{
    std::ostringstream stream;

    stream << value;

    return stream.str();
}

bool CompareFiles(const char * pathA, const char * pathB);
bool CompareFiles(const std::string & pathA, const std::string & pathB);
bool CompareTextFiles(const char * pathA, const char * pathB);
bool CompareTextFiles(const std::string & pathA, const std::string & pathB);
bool CompareLogFiles(const char * pathA, const char * pathB);
bool CompareLogFiles(const std::string & pathA, const std::string & pathB);

bool Compare(double expected, double actual, double epsilonAbs, double epsilonRel);
bool Compare(float expected, float actual, float epsilonAbs, float epsilonRel);

template<class T>
bool Compare(T const * expected, int offsetExpected,
             T const * actual, int offsetActual, size_t numSamples)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    for (size_t i = 0; i < numSamples; i++)
    {
        if (expected[i + offsetExpected] != actual[i + offsetActual])
            return false;
    }
    return true;
}

template<class T>
bool Compare(T const * expected, T const * actual, size_t numSamples)
{
    return Compare(expected, 0, actual, 0, numSamples);
}

template<class T>
bool Compare(std::vector<T> const & expected, int offsetExpected,
             std::vector<T> const & actual, int offsetActual, size_t numSamples)
{
    if ((expected.size() < offsetExpected + numSamples) || (actual.size() < offsetActual + numSamples))
    {
        throw std::runtime_error("Arrays not large enough");
    }
    return Compare(expected.data(), offsetExpected, actual.data(), offsetActual, numSamples);
}

template<class T>
bool Compare(std::vector<T> const & expected, std::vector<T> const & actual, size_t numSamples)
{
    if ((expected.size() < numSamples) || (actual.size() < numSamples))
    {
        throw std::runtime_error("Arrays not large enough");
    }
    return Compare(expected.data(), 0, actual.data(), 0, numSamples);
}

template<class T>
bool Compare(std::vector<T> const & expected, std::vector<T> const & actual)
{
    if (expected.size() != actual.size())
    {
        throw std::runtime_error("Arrays not equal size");
    }
    return Compare(expected.data(), actual.data(), expected.size());
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * expected, int offsetExpected,
        T const * actual, int offsetActual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    if ((offsetExpected < 0) || (offsetActual < 0) || (numSamples < 0))
    {
        return false;
    }
    for (int i = 0; i < numSamples; i++)
    {
        if (!Compare(expected[i + offsetExpected],
                     actual[i + offsetActual],
                     epsilonAbs, epsilonRel))
            return false;
    }
    return true;
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * expected, T const * actual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    return Compare(expected, 0, actual, 0, numSamples, epsilonAbs, epsilonRel);
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(std::vector<T> const & expected, int offsetExpected,
        std::vector<T> const & actual, int offsetActual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    if ((expected.size() < offsetExpected + numSamples) || (actual.size() < offsetActual + numSamples))
    {
        throw std::runtime_error("Arrays not large enough");
    }
    return Compare(expected.data(), offsetExpected, actual.data(), offsetActual, numSamples, epsilonAbs, epsilonRel);
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(std::vector<T> const & expected, std::vector<T> const & actual, int numSamples,
        T epsilonAbs, T epsilonRel)
{
    if ((expected.size() < numSamples) || (actual.size() < numSamples))
    {
        throw std::runtime_error("Arrays not large enough");
    }
    return Compare(expected.data(), 0, actual.data(), 0, numSamples, epsilonAbs, epsilonRel);
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(std::vector<T> const & expected, std::vector<T> const & actual,
        T epsilonAbs, T epsilonRel)
{
    if (expected.size() != actual.size())
    {
        throw std::runtime_error("Arrays not equal size");
    }
    return Compare(expected.data(), 0, actual.data(), 0, expected.size());
}

template<class T>
bool Compare(T const * const * expected, int offsetExpectedRows, int offsetExpectedCols,
             T const * const * actual, int offsetActualRows, int offsetActualCols,
             int numRows, int numCols)
{
    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
        (offsetActualRows < 0) || (offsetActualCols < 0) ||
        (numRows < 0) || (numCols < 0))
    {
        return false;
    }
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (expected[i + offsetExpectedRows][j + offsetExpectedCols] !=
                actual[i + offsetActualRows][j + offsetActualCols])
                return false;
        }
    }
    return true;
}

template<class T>
bool Compare(T const * const * expected, T const * const * actual, int numRows, int numCols)
{
    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols);
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * const * expected, int offsetExpectedRows, int offsetExpectedCols,
        T const * const * actual, int offsetActualRows, int offsetActualCols,
        int numRows, int numCols,
        T epsilonAbs, T epsilonRel)
{
    if ((offsetExpectedRows < 0) || (offsetExpectedCols < 0) ||
        (offsetActualRows < 0) || (offsetActualCols < 0) ||
        (numRows < 0) || (numCols < 0))
    {
        return false;
    }
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (!Compare(expected[i + offsetExpectedRows][j + offsetExpectedCols],
                         actual[i + offsetActualRows][j + offsetActualCols],
                         epsilonAbs, epsilonRel))
                return false;
        }
    }
    return true;
}

template<class T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Compare(T const * const * expected, T const * const * actual,
        int numRows, int numCols,
        T epsilonAbs, T epsilonRel)
{
    return Compare(expected, 0, 0, actual, 0, 0, numRows, numCols, epsilonAbs, epsilonRel);
}

std::streambuf & serialize_out(std::streambuf & sb, void * p, int size);
std::streambuf & serialize_in(std::streambuf & sb, void * p, int size);

void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode);

} // namespace Util

} // namespace Core

