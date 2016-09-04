#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Assembler
{

class Buffer
{
public:
    static const size_t MinBufferSize;
    static const size_t MaxBufferSize;
    static const wchar_t EndOfFile;

    Buffer(Buffer const &) = delete;
    Buffer(std::istream * stream, bool isUserOwned, bool isUTF8 = false);
    Buffer(std::string const & path, bool isUTF8 = false);
    virtual ~Buffer();

	virtual wchar_t Read();
	virtual wchar_t Peek();
	virtual std::wstring ReadString(size_t length);
	virtual std::wstring PeekString(size_t length);
	virtual std::wstring PeekString(size_t beg, size_t end);
	virtual size_t GetPos();
	virtual void SetPos(size_t newPos);

private:
	std::vector<char> buf;
	size_t bufCapacity;
	size_t bufStart;
	size_t bufLen;
	size_t fileLen;
	size_t bufPos;
	std::istream * stream;
    bool isUserOwned;
    bool isUTF8;

    void SetupBuffer(std::istream * stream, bool isUserOwned);
	bool CanSeek();
    size_t ReadNextBlock();
    wchar_t ReadChar();
};

} // namespace Assembler
