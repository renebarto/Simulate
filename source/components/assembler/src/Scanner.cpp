#include "Scanner.h"

namespace Assembler
{

const wchar_t Scanner::EOL = L'\n';
const wchar_t Scanner::Plus = L'+';
const wchar_t Scanner::Minus = L'-';
const wchar_t Scanner::Asterisk = L'*';
const wchar_t Scanner::Slash = L'/';
const wchar_t Scanner::Comma = L',';
const wchar_t Scanner::ParenthesisOpen = L'(';
const wchar_t Scanner::ParenthesisClose = L')';
const wchar_t Scanner::SingleQuote = L'\'';
const wchar_t Scanner::DoubleQuote = L'"';
const wchar_t Scanner::Ampersand = L'&';
const wchar_t Scanner::Colon = L':';
const wchar_t Scanner::Dollar = L'$';
const wchar_t Scanner::At = L'@';
const wchar_t Scanner::QuestionMark = L'?';
const wchar_t Scanner::Equals = L'=';
const wchar_t Scanner::AngleBracketOpen = L'<';
const wchar_t Scanner::AngleBracketClose = L'>';
const wchar_t Scanner::Percent = L'%';
const wchar_t Scanner::ExclamationMark = L'!';
const wchar_t Scanner::Semicolon = L';';
const wchar_t Scanner::Dot = L'.';
const wchar_t Scanner::BackSlash = L'\\';
const wchar_t Scanner::Space = L' ';
const wchar_t Scanner::Tab = L'\t';
const wchar_t Scanner::CR = L'\r';
const wchar_t Scanner::LF = L'\n';
const wchar_t Scanner::FF = L'\f';

Scanner::Scanner(std::istream * stream, bool isUserOwned)
    : currentChar()
	, bufferPos()
	, location()
    , buffer(stream, isUserOwned, true)
    , assemblerKeywords()
    , startStates()
    , queuedTokens()
{
    Init();
}

Scanner::Scanner(std::string const & path)
    : currentChar()
	, bufferPos()
	, location()
    , buffer(path, true)
    , assemblerKeywords()
    , startStates()
    , queuedTokens()
{
    Init();
}

void Scanner::Init()
{
	for (wchar_t i = L'A'; i <= L'Z'; ++i) startStates.Set(i, CharType::Letter);
	startStates.Set(L'_', CharType::Letter);
	for (wchar_t i = L'a'; i <= L'z'; ++i) startStates.Set(i, CharType::Letter);
	for (wchar_t i = L'0'; i <= L'9'; ++i) startStates.Set(i, CharType::Digit);
    startStates.Set(Plus, CharType::Plus);
    startStates.Set(Minus, CharType::Minus);
    startStates.Set(Asterisk, CharType::Asterisk);
    startStates.Set(Slash, CharType::Slash);
    startStates.Set(Comma, CharType::Comma);
    startStates.Set(ParenthesisOpen, CharType::ParenthesisOpen);
    startStates.Set(ParenthesisClose, CharType::ParenthesisClose);
	startStates.Set(SingleQuote, CharType::SingleQuote);
	startStates.Set(DoubleQuote, CharType::DoubleQuote);
    startStates.Set(Ampersand, CharType::Ampersand);
	startStates.Set(Colon, CharType::Colon);
    startStates.Set(Dollar, CharType::Dollar);
    startStates.Set(At, CharType::At);
    startStates.Set(QuestionMark, CharType::QuestionMark);
    startStates.Set(Equals, CharType::Equals);
    startStates.Set(AngleBracketOpen, CharType::AngleBracketOpen);
    startStates.Set(AngleBracketClose, CharType::AngleBracketClose);
    startStates.Set(Percent, CharType::Percent);
    startStates.Set(ExclamationMark, CharType::ExclamationMark);
	startStates.Set(Semicolon, CharType::Semicolon);
    startStates.Set(Dot, CharType::Dot);
    startStates.Set(BackSlash, CharType::Backslash);
	startStates.Set(Buffer::EndOfFile, CharType::EndOfFile);
	startStates.Set(EOL, CharType::EOL);

    assemblerKeywords.Set(L"ORG", TokenType::ORGCommand);
	assemblerKeywords.Set(L"END", TokenType::ENDCommand);
	assemblerKeywords.Set(L"EOT", TokenType::EOTCommand);
	assemblerKeywords.Set(L"SET", TokenType::SETCommand);
	assemblerKeywords.Set(L"EQU", TokenType::EQUCommand);
	assemblerKeywords.Set(L"MACRO", TokenType::MACROCommand);
	assemblerKeywords.Set(L"ENDM", TokenType::ENDMCommand);
	assemblerKeywords.Set(L"DB", TokenType::DBCommand);
	assemblerKeywords.Set(L"DW", TokenType::DWCommand);
	assemblerKeywords.Set(L"DS", TokenType::DSCommand);
	assemblerKeywords.Set(L"STACK", TokenType::STACKCommand);
	assemblerKeywords.Set(L"MEMORY", TokenType::MEMORYCommand);
	assemblerKeywords.Set(L"IF", TokenType::IFCommand);
	assemblerKeywords.Set(L"ELSE", TokenType::ELSECommand);
	assemblerKeywords.Set(L"ENDIF", TokenType::ENDIFCommand);
	assemblerKeywords.Set(L"REPT", TokenType::REPTCommand);
	assemblerKeywords.Set(L"IRP", TokenType::IRPCommand);
	assemblerKeywords.Set(L"IRPC", TokenType::IRPCCommand);
	assemblerKeywords.Set(L"EXITM", TokenType::EXITMCommand);
	assemblerKeywords.Set(L"MOD", TokenType::MODOperator);
	assemblerKeywords.Set(L"SHR", TokenType::SHROperator);
	assemblerKeywords.Set(L"SHL", TokenType::SHLOperator);
	assemblerKeywords.Set(L"NOT", TokenType::NOTOperator);
	assemblerKeywords.Set(L"AND", TokenType::ANDOperator);
	assemblerKeywords.Set(L"OR", TokenType::OROperator);
	assemblerKeywords.Set(L"XOR", TokenType::XOROperator);
	assemblerKeywords.Set(L"EQ", TokenType::EQOperator);
	assemblerKeywords.Set(L"NE", TokenType::NEOperator);
	assemblerKeywords.Set(L"LT", TokenType::LTOperator);
	assemblerKeywords.Set(L"LE", TokenType::LEOperator);
	assemblerKeywords.Set(L"GT", TokenType::GTOperator);
	assemblerKeywords.Set(L"GE", TokenType::GEOperator);
	assemblerKeywords.Set(L"NUL", TokenType::NULOperator);
	assemblerKeywords.Set(L"HIGH", TokenType::HIGHOperator);
	assemblerKeywords.Set(L"LOW", TokenType::LOWOperator);
	assemblerKeywords.Set(L"ASEG", TokenType::ASEGDirective);
	assemblerKeywords.Set(L"CSEG", TokenType::CSEGDirective);
	assemblerKeywords.Set(L"DSEG", TokenType::DSEGDirective);
	assemblerKeywords.Set(L"PUBLIC", TokenType::PUBLICDirective);
	assemblerKeywords.Set(L"EXTRN", TokenType::EXTRNDirective);
	assemblerKeywords.Set(L"NAME", TokenType::NAMEDirective);
	assemblerKeywords.Set(L"STKLN", TokenType::STKLNDirective);
	assemblerKeywords.Set(L"LOCAL", TokenType::LOCALDirective);
	assemblerKeywords.Set(L"PAGE", TokenType::PAGEDirective);
	assemblerKeywords.Set(L"INPAGE", TokenType::INPAGEDirective);
	assemblerKeywords.Set(L"CPU", TokenType::CPUDirective);

	for (wchar_t i = L'0'; i <= L'1'; ++i) binaryNumber.Add(i);
	for (wchar_t i = L'0'; i <= L'7'; ++i) octalNumber.Add(i);
	for (wchar_t i = L'0'; i <= L'9'; ++i) decimalNumber.Add(i);
	for (wchar_t i = L'0'; i <= L'9'; ++i) hexNumber.Add(i);
	for (wchar_t i = L'A'; i <= L'F'; ++i) hexNumber.Add(i);
	for (wchar_t i = L'a'; i <= L'f'; ++i) hexNumber.Add(i);
    location = Location(1, 0, -1);
    currentChar = NextCh();
}

bool Scanner::CheckAgainstCharSet(std::wstring const & text, CharSet const & set)
{
    for (auto ch : text)
    {
        if (!set.Contains(ch))
        {
            return false;
        }
    }
    return true;
}

wchar_t Scanner::NextCh()
{
	bufferPos = buffer.GetPos();
	wchar_t ch = buffer.Read();
    ++location;
    if (ch == Buffer::EndOfFile)
        return ch;
    if (ch == CR)
    {
        if (buffer.Peek() == LF)
            buffer.Read();
        ch = EOL;
    }
    else if (ch == LF)
    {
        ch = EOL;
    }
	if (ch == EOL)
    { 
        location.NewLine(); 
    }
    return ch;
}

Token Scanner::NextToken()
{
	while ((currentChar == Space) || (currentChar == Tab))
        currentChar = NextCh();
    Token token;
	token.bufferPos = bufferPos; 
    token.location = location;
    std::wstring tokenValue(1, currentChar);
    CharType state = CharType(startStates.Get(currentChar));
  	TokenType tokenType = TokenType::Unknown;

    switch (state)
    {
    case CharType::EndOfFile:
        {
            token.kind = TokenType::EndOfFile;
            break;
        }
    case CharType::EOL:
        {
            currentChar = NextCh();
            token.kind = TokenType::EOL;
            break;
        }
    case CharType::Letter:
        {
            tokenType = TokenType::Identifier;
            CharType type{};
            do
            {
                currentChar = NextCh();
                type = CharType(startStates.Get(currentChar));
                if ((type == CharType::Letter) || (type == CharType::Digit))
                {
                    tokenValue += currentChar;
                }
            } while ((type == CharType::Letter) || (type == CharType::Digit));
            token.kind = assemblerKeywords.Get(tokenValue, tokenType);
            token.value = tokenValue;
            break;
        }
    case CharType::Percent:
        {
            tokenType = TokenType::Number;
            CharType type{};
            bool done = false;
            while (!done)
            {
                currentChar = NextCh();
                type = CharType(startStates.Get(currentChar));
                if ((type == CharType::Digit) && binaryNumber.Contains(currentChar))
                    tokenValue += currentChar;
                else
                    done = true;
            }
            token.kind = tokenType;
            token.value = tokenValue;
            break;
        }
    case CharType::At:
        {
            tokenType = TokenType::Number;
            CharType type{};
            bool done = false;
            while (!done)
            {
                currentChar = NextCh();
                type = CharType(startStates.Get(currentChar));
                if ((type == CharType::Digit) && octalNumber.Contains(currentChar))
                    tokenValue += currentChar;
                else
                    done = true;
            }
            token.kind = tokenType;
            token.value = tokenValue;
            break;
        }
    case CharType::Dollar:
        {
            CharType type{};
            bool done = false;
            std::wstring hexDigits;
            while (!done)
            {
                currentChar = NextCh();
                type = CharType(startStates.Get(currentChar));
                if (((type == CharType::Digit) || (type == CharType::Letter)) && hexNumber.Contains(currentChar))
                    hexDigits += currentChar;
                else
                    done = true;
            }
            token.kind = (hexDigits.empty()) // Single $ is location counter
                         ? TokenType::LocCounter
                         : TokenType::Number;
            token.value = tokenValue + hexDigits;
            break;
        }
    case CharType::Digit:
        {
            tokenType = TokenType::Number;
            CharType type{};
            bool done = false;
            while (!done)
            {
                currentChar = NextCh();
                type = CharType(startStates.Get(currentChar));
                if (((type == CharType::Digit) || (type == CharType::Letter)) && hexNumber.Contains(currentChar))
                    tokenValue += currentChar;
                else
                {
                    // Special case: B or b is also considered a value hex digit
                    if (towupper(tokenValue[tokenValue.length() - 1]) == 'B')
                    {
                        std::wstring val = tokenValue.substr(0, tokenValue.length() - 1);
                        if (!CheckAgainstCharSet(val, binaryNumber))
                        {
                            tokenType = TokenType::Unknown;
                        }
                    }
                    else if ((towupper(currentChar) == 'O') || (towupper(currentChar) == 'Q'))
                    {
                        if (!CheckAgainstCharSet(tokenValue, octalNumber))
                        {
                            tokenType = TokenType::Unknown;
                        }
                        tokenValue += currentChar;
                        currentChar = NextCh();
                    }
                    else if (towupper(currentChar) == 'H')
                    {
                        if (!CheckAgainstCharSet(tokenValue, hexNumber))
                        {
                            tokenType = TokenType::Unknown;
                        }
                        tokenValue += currentChar;
                        currentChar = NextCh();
                    }
                    else
                    {
                        if (!CheckAgainstCharSet(tokenValue, decimalNumber))
                        {
                            tokenType = TokenType::Unknown;
                        }
                    }
                    done = true;
                }
            }
            token.kind = tokenType;
            token.value = tokenValue;
            break;
        }
    case CharType::Colon:
        {
            tokenType = TokenType::Colon;
            token.kind = tokenType;
            token.value = currentChar;
            currentChar = NextCh();
            break;
        }
    case CharType::Comma:
        {
            tokenType = TokenType::Comma;
            token.kind = tokenType;
            token.value = currentChar;
            currentChar = NextCh();
            break;
        }
    case CharType::DoubleQuote:
        {
            tokenType = TokenType::String;
            CharType type{};
            for (;;)
            {
                currentChar = NextCh();
                if ((currentChar == Buffer::EndOfFile) || (currentChar == EOL))
                {
                    tokenType = TokenType::BadString;
                    break;
                }
                else if (currentChar == DoubleQuote)
                {
                    tokenValue += currentChar;
                    currentChar = NextCh();
                    break;
                }
                else if (currentChar == BackSlash)
                {
                    tokenValue += currentChar;
                    currentChar = NextCh();
                    if ((currentChar >= L' ' && currentChar <= L'~'))
                    {
                        tokenValue += currentChar;
                    }
                }
                else
                {
                    tokenValue += currentChar;
                }
            }
            token.kind = tokenType;
            token.value = tokenValue;
            break;
        }
    case CharType::SingleQuote:
        {
            tokenType = TokenType::String;
            CharType type{};
            for (;;)
            {
                currentChar = NextCh();
                if ((currentChar == Buffer::EndOfFile) || (currentChar == EOL))
                {
                    tokenType = TokenType::BadString;
                    break;
                }
                else if (currentChar == SingleQuote)
                {
                    tokenValue += currentChar;
                    currentChar = NextCh();
                    break;
                }
                else if (currentChar == BackSlash)
                {
                    tokenValue += currentChar;
                    currentChar = NextCh();
                    if ((currentChar >= L' ' && currentChar <= L'~'))
                    {
                        tokenValue += currentChar;
                    }
                }
                else
                {
                    tokenValue += currentChar;
                }
            }
            token.kind = tokenType;
            token.value = tokenValue;
            break;
        }
    case CharType::Semicolon:
        {
            tokenValue.clear();
            currentChar = NextCh();
	        for (;;)
            {
		        if (currentChar == EOL)
                {
                    break;
		        }
                else if (currentChar == Buffer::EndOfFile)
                {
                    break;
                }
                else
                {
                    tokenValue += currentChar;
                    currentChar = NextCh();
                }
	        }
            token.kind = TokenType::Comment;
            token.value = tokenValue;
            break;
        }
    }
    return token;
}

Token Scanner::Scan()
{
	if (queuedTokens.size() > 0)
    {
		Token token = queuedTokens.front();
        queuedTokens.pop_front();
        return token;
	}
    else
    {
        return NextToken();
	}
}

void  Scanner::Pushback(Token const & token)
{
    queuedTokens.push_back(token);
}

} // namespace Assembler
