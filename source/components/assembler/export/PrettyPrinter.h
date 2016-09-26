#pragma once

#include <ostream>
#include <string>
#include <streambuf>

namespace Assembler
{

template<class CharType>
class PrettyPrinterBuf: public std::basic_streambuf<CharType, std::char_traits<CharType>>
{
public:
    PrettyPrinterBuf(std::basic_streambuf<CharType, std::char_traits<CharType>> * sb)
        : sb(sb)
        , line(1)
        , column(1)
    {
    }

    size_t Column() { return column; }
    size_t Line() { return line; }

private:
    virtual int_type overflow(int_type c) override
    {
        if (c == EOF)
        {
            return !EOF;
        }
        else if (HaveNewLine(c))
        {
            NewLine();
            return sb->sputc(c);
        }
        else
        {
            ++column;
            return sb->sputc(c);
        }
    }
    
    virtual int sync()
    {
        return sb->pubsync();
    }

    bool HaveNewLine(CharType c);
    void NewLine()
    {
        column = 1;
        ++line;
    }

private:
    std::basic_streambuf<CharType, std::char_traits<CharType>> * sb;
    size_t line;
    size_t column;
};

inline bool PrettyPrinterBuf<char>::HaveNewLine(char c)
{
    return (c == '\n');
}

inline bool PrettyPrinterBuf<wchar_t>::HaveNewLine(wchar_t c)
{
    return (c == L'\n');
}

template<class CharType>
class PrettyPrinterStream : public std::basic_ostream<CharType, std::char_traits<CharType>>
{
public:
    PrettyPrinterStream(std::basic_ostream<CharType, std::char_traits<CharType>> & stream)
        : std::basic_ostream<CharType, std::char_traits<CharType>>(&buf)
        , buf(stream.rdbuf())
    {
    }
    size_t Column() { return buf.Column(); }
    size_t Line() { return buf.Line(); }

private:
    PrettyPrinterBuf<CharType> buf;
};

template<class CharType>
class PrettyPrinter
{
public:
    // The type of basic IO manipulators (endl, ends, and flush) for wide streams.
    typedef std::basic_ostream<CharType, std::char_traits<CharType>> & (* BasicIoManip)(std::basic_ostream<CharType, std::char_traits<CharType>> &);

	PrettyPrinter(std::basic_ostream<CharType, std::char_traits<CharType>> & stream)
        : stream(stream)
    {}
    virtual ~PrettyPrinter() {}

    void SetColumn(size_t newColumn)
    {
        size_t column = stream.Column();
        if (newColumn > column)
        {
            stream << std::basic_string<CharType>(newColumn - column, L' ');
        }
    }

    // Streams a non-pointer value to this object.
    template <typename T>
    inline PrettyPrinter & operator << (const T & val)
    {
//        using ::operator <<;
        stream << val;
        return *this;
    }

    inline PrettyPrinter & operator << (BasicIoManip val)
    {
        stream << val;
        return *this;
    }

private:
    PrettyPrinterStream<CharType> stream;
}; // PrettyPrinter

struct _SetColumn
{
    size_t column;
};

template<class CharType>
inline PrettyPrinter<CharType> & operator << (PrettyPrinter<CharType> & stream, _SetColumn value)
{
    stream.SetColumn(value.column);
    return stream;
}

} // namespace Assembler

inline Assembler::_SetColumn column(size_t column)
{
	return{ column };
}

