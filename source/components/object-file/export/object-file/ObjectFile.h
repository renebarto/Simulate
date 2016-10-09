#pragma once

#include <ostream>
#include "object-file/ObjectCode.h"

namespace ObjectFile
{

class ObjectFile
{
public:
    ObjectFile();
    virtual ~ObjectFile();

    void LoadObjectCode(std::istream & stream, ObjectCode & code);
    void LoadObjectCode(std::string const & path, ObjectCode & code);
    void WriteObjectCode(std::ostream & stream, ObjectCode const & code);
    void WriteObjectCode(std::string const & path, ObjectCode const & code);
}; // ObjectFile

} // namespace ObjectFile
