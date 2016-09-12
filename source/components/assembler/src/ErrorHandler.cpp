#include "ErrorHandler.h"

#include <sstream>

namespace Assembler
{

const size_t ErrorHandler::MinErrorDistance = 2;

ErrorHandler::ErrorHandler(std::wostream & stream, AssemblerMessages & messages)
    : stream(stream)
    , messages(messages)
    , numExceptions()
    , numErrors()
    , numWarnings()
    , errorDistance(MinErrorDistance)
{
}

void ErrorHandler::SyntaxError(Location const & location, TokenType tokenType)
{
    std::wstring s;
    switch (tokenType)
    {
    case TokenType::EndOfFile:              s = L"EOF expected"; break;
    case TokenType::Identifier:             s = L"Identifier expected"; break;
    case TokenType::Number:                 s = L"Number expected"; break;
    case TokenType::String:                 s = L"String expected"; break;
    case TokenType::BadString:              s = L"BadString expected"; break;
    case TokenType::Colon:                  s = L"':' expected"; break;
    case TokenType::Comma:                  s = L"',' expected"; break;
    case TokenType::ORGCommand:             s = L"\"ORG\" expected"; break;
    case TokenType::ENDCommand:             s = L"\"END\" expected"; break;
    case TokenType::EOTCommand:             s = L"\"EOT\" expected"; break;
    case TokenType::SETCommand:             s = L"\"SET\" expected"; break;
    case TokenType::EQUCommand:             s = L"\"EQU\" expected"; break;
    case TokenType::MACROCommand:           s = L"\"MACRO\" expected"; break;
    case TokenType::ENDMCommand:            s = L"\"ENDM\" expected"; break;
    case TokenType::DBCommand:              s = L"\"DB\" expected"; break;
    case TokenType::DWCommand:              s = L"\"DW\" expected"; break;
    case TokenType::DSCommand:              s = L"\"DS\" expected"; break;
    case TokenType::STACKCommand:           s = L"\"STACK\" expected"; break;
    case TokenType::MEMORYCommand:          s = L"\"MEMORY\" expected"; break;
    case TokenType::IFCommand:              s = L"\"IF\" expected"; break;
    case TokenType::ELSECommand:            s = L"\"ELSE\" expected"; break;
    case TokenType::ENDIFCommand:           s = L"\"ENDIF\" expected"; break;
    case TokenType::REPTCommand:            s = L"\"REPT\" expected"; break;
    case TokenType::IRPCommand:             s = L"\"IRPC\" expected"; break;
    case TokenType::IRPCCommand:            s = L"\"IRP\" expected"; break;
    case TokenType::EXITMCommand:           s = L"\"EXITM\" expected"; break;
    case TokenType::MODOperator:            s = L"\"MOD\" expected"; break;
    case TokenType::SHROperator:            s = L"\"SHR\" expected"; break;
    case TokenType::SHLOperator:            s = L"\"SHL\" expected"; break;
    case TokenType::NOTOperator:            s = L"\"NOT\" expected"; break;
    case TokenType::ANDOperator:            s = L"\"AND\" expected"; break;
    case TokenType::OROperator:             s = L"\"OR\" expected"; break;
    case TokenType::XOROperator:            s = L"\"XOR\" expected"; break;
    case TokenType::EQOperator:             s = L"\"EQ\" expected"; break;
    case TokenType::NEOperator:             s = L"\"NE\" expected"; break;
    case TokenType::LTOperator:             s = L"\"LT\" expected"; break;
    case TokenType::LEOperator:             s = L"\"LE\" expected"; break;
    case TokenType::GTOperator:             s = L"\"GT\" expected"; break;
    case TokenType::GEOperator:             s = L"\"GE\" expected"; break;
    case TokenType::NULOperator:            s = L"\"NUL\" expected"; break;
    case TokenType::HIGHOperator:           s = L"\"HIGH\" expected"; break;
    case TokenType::LOWOperator:            s = L"\"LOW\" expected"; break;
    case TokenType::ASEGDirective:          s = L"\"ASEG\" expected"; break;
    case TokenType::CSEGDirective:          s = L"\"CSEG\" expected"; break;
    case TokenType::DSEGDirective:          s = L"\"DSEG\" expected"; break;
    case TokenType::PUBLICDirective:        s = L"\"PUBLIC\" expected"; break;
    case TokenType::EXTRNDirective:         s = L"\"EXTRN\" expected"; break;
    case TokenType::NAMEDirective:          s = L"\"NAME\" expected"; break;
    case TokenType::STKLNDirective:         s = L"\"STKLN\" expected"; break;
    case TokenType::LOCALDirective:         s = L"\"LOCAL\" expected"; break;
    case TokenType::PAGEDirective:          s = L"\"PAGE\" expected"; break;
    case TokenType::INPAGEDirective:        s = L"\"INPAGE\" expected"; break;
    case TokenType::CPUDirective:           s = L"\"CPU\" expected"; break;
    default:
        {
            std::wostringstream formatStream;
            formatStream << "error " << int(tokenType);
            s = formatStream.str();
        }
        break;
    }
    Error(location, s);
}

void ErrorHandler::Error(Location const & location, std::wstring const & s)
{
    if (errorDistance >= MinErrorDistance) 
        stream << L"Error: " << location.GetLine() << ":" << location.GetColumn() << " - " << s << std::endl;
    messages.push_back(AssemblerMessage(location, s));
    ++numErrors;
    errorDistance = 0;
}

void ErrorHandler::Warning(Location const & location, std::wstring const & s)
{
    stream << L"Warning: " << location.GetLine() << ":" << location.GetColumn() << " - " << s << std::endl;
    messages.push_back(AssemblerMessage(location, s));
    ++numWarnings;
}

void ErrorHandler::Warning(std::wstring const & s)
{
    stream << L"Warning: " << s << std::endl;
    messages.push_back(AssemblerMessage(Location(), s));
    ++numWarnings;
}

void ErrorHandler::Exception(std::wstring const & s)
{
    if (errorDistance >= MinErrorDistance) 
        stream << L"Exception: " << s << std::endl;
    messages.push_back(AssemblerMessage(Location(), s));
    ++numExceptions;
    errorDistance = 0;
}

void ErrorHandler::NoError()
{
    ++errorDistance;
}

} // namespace Assembler

