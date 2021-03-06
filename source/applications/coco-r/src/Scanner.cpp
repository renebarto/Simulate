/*----------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-----------------------------------------------------------------------*/


#include <memory.h>
#include <string.h>
#include "Scanner.h"

namespace Coco
{

Token::Token()
{
	kind = 0;
	pos  = 0;
	col  = 0;
	line = 0;
    val = nullptr;
	next = nullptr;
}

Token::~Token()
{
	delete [] val;
}

Buffer::Buffer(FILE* s, bool isUserStream)
{
// ensure binary read on windows
#if _MSC_VER >= 1300
	_setmode(_fileno(s), _O_BINARY);
#endif
	stream = s; this->isUserStream = isUserStream;
	if (CanSeek())
    {
		fseek(s, 0, SEEK_END);
		fileLen = ftell(s);
		fseek(s, 0, SEEK_SET);
		bufLen = (fileLen < COCO_MAX_BUFFER_LENGTH) ? fileLen : COCO_MAX_BUFFER_LENGTH;
		bufStart = INT_MAX; // nothing in the buffer so far
	}
    else 
    {
		fileLen = bufLen = bufStart = 0;
	}
	bufCapacity = (bufLen>0) ? bufLen : COCO_MIN_BUFFER_LENGTH;
	buf = new unsigned char[bufCapacity];	
	if (fileLen > 0) 
        SetPos(0);          // setup  buffer to position 0 (start)
	else
        bufPos = 0; // index 0 is already after the file, thus Pos = 0 is invalid
	if (bufLen == fileLen && CanSeek())
        Close();
}

Buffer::Buffer(Buffer *b)
{
	buf = b->buf;
	bufCapacity = b->bufCapacity;
	b->buf = nullptr;
	bufStart = b->bufStart;
	bufLen = b->bufLen;
	fileLen = b->fileLen;
	bufPos = b->bufPos;
	stream = b->stream;
	b->stream = nullptr;
	isUserStream = b->isUserStream;
}

Buffer::Buffer(unsigned char const * buf, size_t len)
{
	this->buf = new unsigned char[len];
	memcpy(this->buf, buf, len * sizeof(unsigned char));
	bufStart = 0;
	bufCapacity = bufLen = len;
	fileLen = len;
	bufPos = 0;
	stream = nullptr;
}

Buffer::~Buffer()
{
	Close(); 
	if (buf != nullptr)
    {
		delete [] buf;
		buf = nullptr;
	}
}

void Buffer::Close()
{
	if (!isUserStream && stream != nullptr)
    {
		fclose(stream);
		stream = nullptr;
	}
}

int Buffer::Read()
{
	if (bufPos < bufLen)
    {
		return buf[bufPos++];
	} 
    else if (GetPos() < fileLen)
    {
		SetPos(GetPos()); // shift buffer start to Pos
		return buf[bufPos++];
	} 
    else if ((stream != nullptr) && !CanSeek() && (ReadNextStreamChunk() > 0))
    {
		return buf[bufPos++];
	} 
    else 
    {
		return EoF;
	}
}

int Buffer::Peek()
{
	size_t curPos = GetPos();
	int ch = Read();
	SetPos(curPos);
	return ch;
}

// beg .. begin, zero-based, inclusive, in byte
// end .. end, zero-based, exclusive, in byte
std::wstring Buffer::GetString(size_t beg, size_t end)
{
	size_t len = 0;
	wchar_t *buf = new wchar_t[end - beg];
	size_t oldPos = GetPos();
	SetPos(beg);
	while (GetPos() < end) 
        buf[len++] = (wchar_t) Read();
	SetPos(oldPos);
	return String::Create(buf, 0, len);
}

size_t Buffer::GetPos()
{
	return bufPos + bufStart;
}

void Buffer::SetPos(size_t value)
{
	if ((value >= fileLen) && (stream != nullptr) && !CanSeek())
    {
		// Wanted position is after buffer and the stream
		// is not seek-able e.g. network or console,
		// thus we have to read the stream manually till
		// the wanted position is in sight.
		while ((value >= fileLen) && (ReadNextStreamChunk() > 0));
	}

	if ((value < 0) || (value > fileLen))
    {
		wprintf(L"--- buffer out of bounds access, position: %d\n", int(value));
		exit(1);
	}

	if ((value >= bufStart) && (value < (bufStart + bufLen)))
    { // already in buffer
		bufPos = value - bufStart;
	} 
    else if (stream != nullptr)
    { // must be swapped in
		fseek(stream, long(value), SEEK_SET);
		bufLen = fread(buf, sizeof(unsigned char), bufCapacity, stream);
		bufStart = value; bufPos = 0;
	} 
    else 
    {
		bufPos = fileLen - bufStart; // make Pos return fileLen
	}
}

// Read the next chunk of bytes from the stream, increases the buffer
// if needed and updates the fields fileLen and bufLen.
// Returns the number of bytes read.
size_t Buffer::ReadNextStreamChunk()
{
	size_t free = bufCapacity - bufLen;
	if (free == 0)
    {
		// in the case of a growing input stream
		// we can neither seek in the stream, nor can we
		// foresee the maximum length, thus we must adapt
		// the buffer size on demand.
		bufCapacity = bufLen * 2;
		unsigned char *newBuf = new unsigned char[bufCapacity];
		memcpy(newBuf, buf, bufLen*sizeof(unsigned char));
		delete [] buf;
		buf = newBuf;
		free = bufLen;
	}
	size_t read = fread(buf + bufLen, sizeof(unsigned char), free, stream);
	if (read > 0) 
    {
		fileLen = bufLen = (bufLen + read);
		return read;
	}
	// end of stream reached
	return 0;
}

bool Buffer::CanSeek()
{
	return (stream != nullptr) && (ftell(stream) != -1);
}

int UTF8Buffer::Read()
{
	int ch;
	do
    {
		ch = Buffer::Read();
		// until we find a utf8 start (0xxxxxxx or 11xxxxxx)
	} while ((ch >= 128) && ((ch & 0xC0) != 0xC0) && (ch != EoF));
	if (ch < 128 || ch == EoF)
    {
		// nothing to do, first 127 chars are the same in ascii and utf8
		// 0xxxxxxx or end of file character
	}
    else if ((ch & 0xF0) == 0xF0)
    {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		int c1 = ch & 0x07; ch = Buffer::Read();
		int c2 = ch & 0x3F; ch = Buffer::Read();
		int c3 = ch & 0x3F; ch = Buffer::Read();
		int c4 = ch & 0x3F;
		ch = (((((c1 << 6) | c2) << 6) | c3) << 6) | c4;
	} 
    else if ((ch & 0xE0) == 0xE0)
    {
		// 1110xxxx 10xxxxxx 10xxxxxx
		int c1 = ch & 0x0F; ch = Buffer::Read();
		int c2 = ch & 0x3F; ch = Buffer::Read();
		int c3 = ch & 0x3F;
		ch = (((c1 << 6) | c2) << 6) | c3;
	} 
    else if ((ch & 0xC0) == 0xC0)
    {
		// 110xxxxx 10xxxxxx
		int c1 = ch & 0x1F; ch = Buffer::Read();
		int c2 = ch & 0x3F;
		ch = (c1 << 6) | c2;
	}
	return ch;
}

Scanner::Scanner(unsigned char const * buf, size_t len) 
{
	buffer = new Buffer(buf, len);
	Init();
}

Scanner::Scanner(std::string const & fileName)
{
	FILE* stream;
	if ((stream = fopen(fileName.c_str(), "rb")) == nullptr)
    {
		wprintf(L"--- Cannot open file %hs\n", fileName.c_str());
		exit(1);
	}
	buffer = new Buffer(stream, false);
	Init();
}

Scanner::Scanner(FILE* s)
{
	buffer = new Buffer(s, true);
	Init();
}

Scanner::~Scanner()
{
	char* cur = (char*) firstHeap;

	while(cur != nullptr)
    {
		cur = *(char**) (cur + COCO_HEAP_BLOCK_SIZE);
		free(firstHeap);
		firstHeap = cur;
	}
	delete [] tval;
	delete buffer;
}

void Scanner::Init()
{
	EOL    = '\n';
	eofSym = 0;
	maxT = 41;
	noSym = 41;
	int i;
	for (i = 65; i <= 90; ++i) start.set(i, 1);
	for (i = 95; i <= 95; ++i) start.set(i, 1);
	for (i = 97; i <= 122; ++i) start.set(i, 1);
	for (i = 48; i <= 57; ++i) start.set(i, 2);
	start.set(34, 12);
	start.set(39, 5);
	start.set(36, 13);
	start.set(61, 16);
	start.set(46, 31);
	start.set(43, 17);
	start.set(45, 18);
	start.set(60, 32);
	start.set(62, 20);
	start.set(124, 23);
	start.set(40, 33);
	start.set(41, 24);
	start.set(91, 25);
	start.set(93, 26);
	start.set(123, 27);
	start.set(125, 28);
	start.set(Buffer::EoF, -1);
	keywords.set(L"COMPILER", 6);
	keywords.set(L"IGNORECASE", 7);
	keywords.set(L"CHARACTERS", 8);
	keywords.set(L"TOKENS", 9);
	keywords.set(L"PRAGMAS", 10);
	keywords.set(L"COMMENTS", 11);
	keywords.set(L"FROM", 12);
	keywords.set(L"TO", 13);
	keywords.set(L"NESTED", 14);
	keywords.set(L"IGNORE", 15);
	keywords.set(L"PRODUCTIONS", 16);
	keywords.set(L"END", 19);
	keywords.set(L"ANY", 23);
	keywords.set(L"WEAK", 29);
	keywords.set(L"SYNC", 36);
	keywords.set(L"IF", 37);
	keywords.set(L"CONTEXT", 38);


	tvalLength = 128;
	tval = new wchar_t[tvalLength]; // text of current token

	// COCO_HEAP_BLOCK_SIZE byte heap + pointer to next heap block
	heap = malloc(COCO_HEAP_BLOCK_SIZE + sizeof(void*));
	firstHeap = heap;
	heapEnd = (void**) (((char*) heap) + COCO_HEAP_BLOCK_SIZE);
	*heapEnd = 0;
	heapTop = heap;
	if (sizeof(Token) > COCO_HEAP_BLOCK_SIZE)
    {
		wprintf(L"--- Too small COCO_HEAP_BLOCK_SIZE\n");
		exit(1);
	}

	pos = -1; line = 1; col = 0; charPos = -1;
	oldEols = 0;
	NextCh();
	if (ch == 0xEF) { // check optional byte order mark for UTF-8
		NextCh(); int ch1 = ch;
		NextCh(); int ch2 = ch;
		if (ch1 != 0xBB || ch2 != 0xBF)
        {
			wprintf(L"Illegal byte order mark at start of file");
			exit(1);
		}
		Buffer *oldBuf = buffer;
		buffer = new UTF8Buffer(buffer); col = 0; charPos = -1;
		delete oldBuf; oldBuf = nullptr;
		NextCh();
	}


	pt = tokens = CreateToken(); // first token is a dummy
}

void Scanner::NextCh()
{
	if (oldEols > 0)
    {
        ch = EOL; 
        oldEols--;
    }
	else
    {
		pos = buffer->GetPos();
		// buffer reads unicode chars, if UTF8 has been detected
		ch = buffer->Read(); col++; charPos++;
		// replace isolated '\r' by '\n' in order to make
		// eol handling uniform across Windows, Unix and Mac
		if (ch == L'\r' && buffer->Peek() != L'\n')
            ch = EOL;
		if (ch == EOL)
        { 
            line++; 
            col = 0;
        }
	}

}

void Scanner::AddCh()
{
	if (tlen >= tvalLength)
    {
		tvalLength *= 2;
		wchar_t *newBuf = new wchar_t[tvalLength];
		memcpy(newBuf, tval, tlen*sizeof(wchar_t));
		delete [] tval;
		tval = newBuf;
	}
	if (ch != Buffer::EoF)
    {
		tval[tlen++] = ch;
		NextCh();
	}
}


bool Scanner::Comment0()
{
	size_t level = 1, pos0 = pos, line0 = line, col0 = col, charPos0 = charPos;
	NextCh();
	if (ch == L'/')
    {
		NextCh();
		for(;;)
        {
			if (ch == 10)
            {
				level--;
				if (level == 0)
                { 
                    oldEols = line - line0;
                    NextCh();
                    return true;
                }
				NextCh();
			} 
            else if (ch == buffer->EoF) 
                return false;
			else 
                NextCh();
		}
	} 
    else 
    {
		buffer->SetPos(pos0); NextCh(); line = line0; col = col0; charPos = charPos0;
	}
	return false;
}

bool Scanner::Comment1()
{
    int level = 1;
    size_t pos0 = pos, line0 = line, col0 = col, charPos0 = charPos;
	NextCh();
	if (ch == L'*')
    {
		NextCh();
		for(;;)
        {
			if (ch == L'*')
            {
				NextCh();
				if (ch == L'/')
                {
					level--;
					if (level == 0)
                    { 
                        oldEols = line - line0; 
                        NextCh();
                        return true; 
                    }
					NextCh();
				}
			} 
            else if (ch == L'/')
            {
				NextCh();
				if (ch == L'*')
                {
					level++; NextCh();
				}
			} 
            else if (ch == buffer->EoF) 
                return false;
			else 
                NextCh();
		}
	}
    else
    {
		buffer->SetPos(pos0); NextCh(); line = line0; col = col0; charPos = charPos0;
	}
	return false;
}


void Scanner::CreateHeapBlock()
{
	void* newHeap;
	char* cur = (char*) firstHeap;

	while(((char*) tokens < cur) || ((char*) tokens > (cur + COCO_HEAP_BLOCK_SIZE)))
    {
		cur = *((char**) (cur + COCO_HEAP_BLOCK_SIZE));
		free(firstHeap);
		firstHeap = cur;
	}

	// COCO_HEAP_BLOCK_SIZE byte heap + pointer to next heap block
	newHeap = malloc(COCO_HEAP_BLOCK_SIZE + sizeof(void*));
	*heapEnd = newHeap;
	heapEnd = (void**) (((char*) newHeap) + COCO_HEAP_BLOCK_SIZE);
	*heapEnd = 0;
	heap = newHeap;
	heapTop = heap;
}

Token* Scanner::CreateToken()
{
	Token *t;
	if (((char*) heapTop + (int) sizeof(Token)) >= (char*) heapEnd)
    {
		CreateHeapBlock();
	}
	t = (Token*) heapTop;
	heapTop = (void*) ((char*) heapTop + sizeof(Token));
	t->val = nullptr;
	t->next = nullptr;
	return t;
}

void Scanner::AppendVal(Token *t)
{
	size_t reqMem = (tlen + 1) * sizeof(wchar_t);
	if (((char*) heapTop + reqMem) >= (char*) heapEnd) 
    {
		if (reqMem > COCO_HEAP_BLOCK_SIZE)
        {
			wprintf(L"--- Too long token value\n");
			exit(1);
		}
		CreateHeapBlock();
	}
	t->val = (wchar_t*) heapTop;
	heapTop = (void*) ((char*) heapTop + reqMem);

	wcsncpy(t->val, tval, tlen);
	t->val[tlen] = L'\0';
}

Token* Scanner::NextToken()
{
	while (ch == ' ' ||
			(ch >= 9 && ch <= 10) || ch == 13)
        NextCh();
	if ((ch == L'/' && Comment0()) || (ch == L'/' && Comment1())) 
        return NextToken();
	int recKind = noSym;
	size_t recEnd = pos;
	t = CreateToken();
	t->pos = pos; t->col = col; t->line = line; t->charPos = charPos;
	int state = start.state(ch);
	tlen = 0; AddCh();

	switch (state)
    {
		case -1: { t->kind = eofSym; break; } // NextCh already done
		case 0: {
			case_0:
			if (recKind != noSym)
            {
				tlen = recEnd - t->pos;
				SetScannerBehindT();
			}
			t->kind = recKind; break;
		} // NextCh already done
		case 1:
			case_1:
			recEnd = pos; recKind = 1;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'a' && ch <= L'z'))
            {
                AddCh(); 
                goto case_1;
            }
			else 
            {
                t->kind = 1; 
                std::wstring literal = String::Create(tval, 0, tlen); 
                t->kind = keywords.get(literal, t->kind); 
                break;
            }
		case 2:
			case_2:
			recEnd = pos; recKind = 2;
			if ((ch >= L'0' && ch <= L'9'))
            {
                AddCh(); 
                goto case_2;
            }
			else 
            {
                t->kind = 2; 
                break;
            }
		case 3:
			case_3:
			{
                t->kind = 3; 
                break;
            }
		case 4:
			case_4:
			{
                t->kind = 4; 
                break;
            }
		case 5:
			if (ch <= 9 || (ch >= 11 && ch <= 12) || (ch >= 14 && ch <= L'&') || (ch >= L'(' && ch <= L'[') || (ch >= L']' && ch <= 65535))
            {
                AddCh(); 
                goto case_6;
            }
			else if (ch == 92)
            {
                AddCh(); 
                goto case_7;
            }
			else 
            {
                goto case_0;
            }
		case 6:
			case_6:
			if (ch == 39)
            {
                AddCh(); 
                goto case_9;
            }
			else
            {
                goto case_0;
            }
		case 7:
			case_7:
			if ((ch >= L' ' && ch <= L'~'))
            {
                AddCh(); 
                goto case_8;
            }
			else
            {
                goto case_0;
            }
		case 8:
			case_8:
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'a' && ch <= L'f'))
            {
                AddCh(); 
                goto case_8;
            }
			else if (ch == 39)
            {
                AddCh(); 
                goto case_9;
            }
			else
            {
                goto case_0;
            }
		case 9:
			case_9:
			{
                t->kind = 5; 
                break;
            }
		case 10:
			case_10:
			recEnd = pos; recKind = 42;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'a' && ch <= L'z'))
            {
                AddCh(); 
                goto case_10;
            }
			else
            {
                t->kind = 42; 
                break;
            }
		case 11:
			case_11:
			recEnd = pos; recKind = 43;
			if ((ch >= L'-' && ch <= L'.') || (ch >= L'0' && ch <= L':') || (ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'a' && ch <= L'z'))
            {
                AddCh(); 
                goto case_11;
            }
			else 
            {
                t->kind = 43; 
                break;
            }
		case 12:
			case_12:
			if (ch <= 9 || (ch >= 11 && ch <= 12) || (ch >= 14 && ch <= L'!') || (ch >= L'#' && ch <= L'[') || (ch >= L']' && ch <= 65535))
            {
                AddCh(); 
                goto case_12;
            }
			else if (ch == 10 || ch == 13) 
            {
                AddCh(); 
                goto case_4;
            }
			else if (ch == L'"') 
            {
                AddCh(); 
                goto case_3;
            }
			else if (ch == 92) 
            {
                AddCh(); 
                goto case_14;
            }
			else 
            {
                goto case_0;
            }
		case 13:
			recEnd = pos; recKind = 42;
			if ((ch >= L'0' && ch <= L'9'))
            {
                AddCh(); 
                goto case_10;
            }
			else if ((ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'a' && ch <= L'z'))
            {
                AddCh(); 
                goto case_15;
            }
			else 
            {
                t->kind = 42; 
                break;
            }
		case 14:
			case_14:
			if ((ch >= L' ' && ch <= L'~')) 
            {
                AddCh(); 
                goto case_12;
            }
			else
            {
                goto case_0;
            }
		case 15:
			case_15:
			recEnd = pos; recKind = 42;
			if ((ch >= L'0' && ch <= L'9')) 
            {
                AddCh(); 
                goto case_10;
            }
			else if ((ch >= L'A' && ch <= L'Z') || ch == L'_' || (ch >= L'a' && ch <= L'z'))
            {
                AddCh(); 
                goto case_15;
            }
			else if (ch == L'=') 
            {
                AddCh(); 
                goto case_11;
            }
			else
            {
                t->kind = 42; 
                break;
            }
		case 16:
			{
                t->kind = 17; 
                break;
        }
		case 17:
			{
                t->kind = 20; 
                break;
        }
		case 18:
			{
                t->kind = 21; 
                break;
            }
		case 19:
			case_19:
			{
                t->kind = 22; 
                break;
            }
		case 20:
			{
                t->kind = 25; 
                break;
            }
		case 21:
			case_21:
			{
                t->kind = 26; 
                break;
            }
		case 22:
			case_22:
			{
                t->kind = 27; 
                break;
            }
		case 23:
			{
                t->kind = 28; 
                break;
            }
		case 24:
			{
                t->kind = 31; 
                break;
            }
		case 25:
			{
                t->kind = 32; 
                break;
            }
		case 26:
			{
                t->kind = 33; 
                break;
            }
		case 27:
			{
                t->kind = 34; 
                break;
            }
		case 28:
			{
                t->kind = 35; 
                break;
            }
		case 29:
			case_29:
			{
                t->kind = 39; 
                break;
            }
		case 30:
			case_30:
			{
                t->kind = 40; 
                break;
            }
		case 31:
			recEnd = pos; recKind = 18;
			if (ch == L'.')
            {
                AddCh(); 
                goto case_19;
            }
			else if (ch == L'>') 
            {
                AddCh(); 
                goto case_22;
            }
			else if (ch == L')') 
            {
                AddCh(); 
                goto case_30;
            }
			else 
            {
                t->kind = 18; 
                break;
            }
		case 32:
			recEnd = pos; recKind = 24;
			if (ch == L'.')
            {
                AddCh(); 
                goto case_21;
            }
			else 
            {
                t->kind = 24; 
                break;
            }
		case 33:
			recEnd = pos; recKind = 30;
			if (ch == L'.')
            {
                AddCh(); 
                goto case_29;
            }
			else
            {
                t->kind = 30; 
                break;
            }

	}
	AppendVal(t);
	return t;
}

void Scanner::SetScannerBehindT()
{
	buffer->SetPos(t->pos);
	NextCh();
	line = t->line; col = t->col; charPos = t->charPos;
	for (int i = 0; i < tlen; i++)
        NextCh();
}

// get the next token (possibly a token already seen during peeking)
Token* Scanner::Scan()
{
	if (tokens->next == nullptr)
    {
		return pt = tokens = NextToken();
	}
    else
    {
		pt = tokens = tokens->next;
		return tokens;
	}
}

// peek for the next token, ignore pragmas
Token* Scanner::Peek()
{
	do 
    {
		if (pt->next == nullptr)
        {
			pt->next = NextToken();
		}
		pt = pt->next;
	} while (pt->kind > maxT); // skip pragmas

	return pt;
}

// make sure that peeking starts at the current scan position
void Scanner::ResetPeek()
{
	pt = tokens;
}

} // namespace

