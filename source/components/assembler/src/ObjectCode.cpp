#include "ObjectCode.h"

#include <algorithm>

namespace Assembler
{

CodeSegment::CodeSegment(SegmentID id, std::string const & name)
    : id(id)
    , name(name)
    , offset()
    , size()
    , data()
{
}

CodeSegment::~CodeSegment()
{
}

ObjectCode::ObjectCode(std::string const & moduleName)
    : moduleName(moduleName)
    , segments()
{
}

ObjectCode::~ObjectCode()
{
}

void ObjectCode::AddSegment(SegmentID id, std::string const & name)
{
    segments.push_back(CodeSegment(id, name));
}

CodeSegment const & ObjectCode::GetSegment(std::string const & name) const
{
    for (auto & segment : segments)
    {
        if (segment.Name() == name)
        {
            return segment;
        }
    }
    throw std::invalid_argument("Segment not found");
}

CodeSegment & ObjectCode::GetSegment(std::string const & name)
{
    for (auto & segment : segments)
    {
        if (segment.Name() == name)
        {
            return segment;
        }
    }
    throw std::invalid_argument("Segment not found");
}

CodeSegment const & ObjectCode::GetSegment(SegmentID id) const
{
    for (auto & segment : segments)
    {
        if (segment.ID() == id)
        {
            return segment;
        }
    }
    throw std::invalid_argument("Segment not found");
}

CodeSegment & ObjectCode::GetSegment(SegmentID id)
{
    for (auto & segment : segments)
    {
        if (segment.ID() == id)
        {
            return segment;
        }
    }
    throw std::invalid_argument("Segment not found");
}

std::vector<CodeSegment> const & ObjectCode::GetSegments() const
{
    return segments;
}

} // namespace Assembler
