#include "assembler/Buffer.h"

#include <algorithm>
#include <sstream>
#include "assembler/Exceptions.h"

namespace Assembler
{

const size_t Buffer::MinBufferSize = 1024;
const size_t Buffer::MaxBufferSize = 65536;
const wchar_t Buffer::EndOfFile = -1;

Buffer::Buffer(std::istream * stream, bool isUserOwned, bool isUTF8)
    : buf()
    , bufCapacity()
	, bufStart()
	, bufLen()
	, fileLen()
	, bufPos()
    , stream(nullptr)
    , isUserOwned()
    , isUTF8(isUTF8)
{
    SetupBuffer(stream, isUserOwned);
}

Buffer::Buffer(std::string const & path, bool isUTF8)
    : buf()
    , bufCapacity()
	, bufStart()
	, bufLen()
	, fileLen()
	, bufPos()
    , stream(nullptr)
    , isUserOwned()
    , isUTF8(isUTF8)
{
    SetupBuffer(new std::ifstream(path, std::ios::in | std::ios::binary), isUserOwned);
}

Buffer::~Buffer()
{
    if (!isUserOwned)
    {
        delete stream;
        stream = nullptr;
    }
    buf.clear();
}

void Buffer::SetupBuffer(std::istream * stream, bool isUserOwned)
{
    if (stream == nullptr)
    {
        throw AssemblerException("Stream is null");
    }
    if (!stream->good())
    {
        if (!isUserOwned)
            delete stream;
        throw AssemblerException("Stream is invalid");
    }
    this->stream = stream;
    this->isUserOwned = isUserOwned;
	if (CanSeek())
    {
		stream->seekg(0, std::ios_base::end);
		fileLen = stream->tellg();
		stream->seekg(0, std::ios_base::beg);
		bufCapacity = (fileLen < MaxBufferSize) ? fileLen : MaxBufferSize;
		bufStart = 0;
        bufLen = 0;
        bufPos = 0;
	}
    else 
    {
		fileLen = 0;
        bufCapacity = MinBufferSize;
		bufStart = 0;
        bufLen = 0;
        bufPos = 0;
	}
	buf = std::vector<char>(bufCapacity);
    ReadNextBlock();
}

wchar_t Buffer::ReadChar()
{
    unsigned char byte1 = buf[bufPos++];
    if (!isUTF8)
    {
        return wchar_t(byte1);
    }

    wchar_t result;
    if ((byte1 & 0x80) == 0x00)
    {
        return wchar_t(byte1);
    }
    if ((byte1 & 0xE0) == 0xC0)
    {
        // 2 byte code
        if ((byte1 == 0xC0) || (byte1 == 0xC1))
        {
            return EndOfFile;
        }
        result = (byte1 & 0x1F) << 6;
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte2 = buf[bufPos++];
        if ((byte2 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte2 & 0x3F) << 0);
        return result;
    }
    if ((byte1 & 0xF0) == 0xE0)
    {
        // 3 byte code
        result = (byte1 & 0x0F) << 12;
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte2 = buf[bufPos++];
        if ((byte2 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte2 & 0x3F) << 6);
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte3 = buf[bufPos++];
        if ((byte3 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte3 & 0x3F) << 0);
        return result;
    }
    if ((byte1 & 0xF8) == 0xF0)
    {
        // 4 byte code
        if (byte1 >= 0xF5)
        {
            return EndOfFile;
        }
        result = (byte1 & 0x0F) << 18;
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte2 = buf[bufPos++];
        if ((byte2 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte2 & 0x3F) << 12);
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte3 = buf[bufPos++];
        if ((byte3 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte3 & 0x3F) << 6);
        if ((bufPos >= bufLen) && !ReadNextBlock())
        {
            return EndOfFile;
        }
        unsigned char byte4 = buf[bufPos++];
        if ((byte4 & 0xC0) != 0x80)
        {
            return EndOfFile;
        }
        result |= ((byte4 & 0x3F) << 0);
        return result;
    }
    return EndOfFile;
}

wchar_t Buffer::Read()
{
    if ((bufStart == 0) && (bufPos == 0) && (bufLen >= 3))
    {
        if ((buf[0] == char(0xEF)) && (buf[1] == char(0xBB)) && (buf[2] == char(0xBF)))
        {
            // Skip UTF-8 BOM
            bufPos = 3;
            isUTF8 = true;
        }
    }

	if (bufPos < bufLen)
    {
		return ReadChar();
	} 
    else if (GetPos() < fileLen)
    {
		SetPos(GetPos());
		return ReadChar();
	} 
    else if (!CanSeek() && (ReadNextBlock() > 0))
    {
		return ReadChar();
	} 
    else 
    {
		return EndOfFile;
	}
}

wchar_t Buffer::Peek()
{
	size_t curPos = GetPos();
	wchar_t ch = Read();
	SetPos(curPos);
	return ch;
}

std::wstring Buffer::ReadString(size_t length)
{
    std::wstring result;
    size_t count = length;
    while (count--)
    {
        result += Read();
    }
    return result;
}

std::wstring Buffer::PeekString(size_t length)
{
    std::wstring result;
    size_t oldPos = GetPos();
    size_t count = length;
    while (count--)
    {
        result += Read();
    }
    SetPos(oldPos);
    return result;
}

std::wstring Buffer::PeekString(size_t beg, size_t end)
{
    std::wstring result;
	size_t oldPos = GetPos();
	SetPos(beg);
	while (GetPos() < end) 
        result += Read();
	SetPos(oldPos);
	return result;
}

size_t Buffer::GetPos()
{
	return bufPos + bufStart;
}

void Buffer::SetPos(size_t newPos)
{
	if ((newPos >= fileLen) && !CanSeek())
    {
		// Wanted position is after buffer and the stream
		// is not seek-able e.g. network or console,
		// thus we have to read the stream manually till
		// the wanted position is in sight.
		while ((newPos >= fileLen) && (ReadNextBlock() > 0));
	}

	if ((newPos < 0) || (newPos > fileLen))
    {
        std::ostringstream stream;
        stream << "--- buffer out of bounds access, position: " << newPos << std::endl;
        throw AssemblerException(stream.str());
	}

	if ((newPos >= bufStart) && (newPos < (bufStart + bufLen)))
    { // already in buffer
		bufPos = newPos - bufStart;
	} 
    else if (stream)
    {
		stream->seekg(newPos, std::ios_base::beg);
		stream->read(buf.data(), bufCapacity);
        bufLen = stream->gcount();
		bufStart = newPos; 
        bufPos = 0;
	} 
    else 
    {
		bufPos = fileLen - bufStart;
	}
}

// Read the next chunk of bytes from the stream, increases the buffer
// if needed and updates the fields fileLen and bufLen.
// Returns the number of bytes read.
size_t Buffer::ReadNextBlock()
{
	size_t free = bufCapacity - bufLen;
	if (free == 0)
    {
		// in the case of a growing input stream
		// we can neither seek in the stream, nor can we
		// foresee the maximum length, thus we must adapt
		// the buffer size on demand.
		bufCapacity = bufLen * 2;
        buf.resize(bufCapacity);
		free += bufLen;
	}
    stream->read(buf.data() + bufLen, free);
	size_t read = stream->gcount();
	if (read > 0) 
    {
        bufLen += read;
        fileLen = std::max(fileLen, bufLen);
		return read;
	}
	// end of stream reached
	return 0;
}

bool Buffer::CanSeek()
{
	return (stream->tellg() != std::streampos(-1));
}

} // namespace Assembler
