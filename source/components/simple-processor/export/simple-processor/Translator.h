#pragma once

#include <iostream>

namespace Simulate
{

class InputSpecifier
{
public:
    InputSpecifier(std::istream & inputStream)
        : inputStream(inputStream)
    {
    }
protected:
    std::istream & inputStream;
};

class OutputSpecifier
{
public:
    OutputSpecifier(std::ostream & outputStream, std::ostream & errorStream)
        : outputStream(outputStream)
        , errorStream(errorStream)
    {
    }
protected:
    std::ostream & outputStream;
    std::ostream & errorStream;
};

class Translator
{
public:
    Translator(InputSpecifier & inputSpecifier, OutputSpecifier & outputSpecifier);
    virtual ~Translator();

    void CharacterHandler();
    void LexicalAnalyzer();
    void SyntaxAnalyzer();
    void ConstraintAnalyzer();
    void IntermediateCodeGenerator();
    void CodeOptimizer();
    void CodeGenerator();

protected:
    InputSpecifier & inputSpecifier;
    OutputSpecifier & outputSpecifier;
};

} // namespace Simulate
