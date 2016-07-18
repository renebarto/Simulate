#pragma once
#include <Parser.h>

namespace ASM8080
{

class Parser8080 : public Parser
{
public:
    Parser8080(Reader & reader);
    virtual ~Parser8080();

    void Attach(Reader & reader) override;
    bool Parse() override;

protected:
    Reader * reader;
};

} // namespace ASM8080
