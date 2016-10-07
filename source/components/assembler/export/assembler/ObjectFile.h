#pragma once

#include <ostream>
#include "assembler/ObjectCode.h"

namespace Assembler
{

class ObjectFile
{
public:
    ObjectFile(std::ostream * stream, bool isUserOwned = true);
    ObjectFile(std::string const & path);
    virtual ~ObjectFile();

    void WriteObjectCode(ObjectCode const & code);

private:
    std::ostream * stream;
    bool isUserOwned;
}; // ObjectFile

} // namespace Assembler
