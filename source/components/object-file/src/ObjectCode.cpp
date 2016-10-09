#include "object-file/ObjectCode.h"

#include <algorithm>

namespace ObjectFile
{

ObjectCode::ObjectCode(std::string const & moduleName)
    : moduleName(moduleName)
    , segments()
{
    Clear();
}

ObjectCode::~ObjectCode()
{
}

void ObjectCode::Clear()
{
    segments.clear();
    AddSegment(SegmentID::ASEG, "ASEG");
    GetSegment(SegmentID::ASEG).SetOffset(0);
}

void ObjectCode::AddSegment(SegmentID id, std::string const & name)
{
    segments.push_back(CodeSegment<AddressType>(id, name));
}

void ObjectCode::AddSegment(CodeSegment<AddressType> const & segment)
{
    segments.push_back(CodeSegment<AddressType>(segment));
}

void ObjectCode::AddSegment(CodeSegment<AddressType> && segment)
{
    segments.push_back(CodeSegment<AddressType>(segment));
}

bool ObjectCode::HaveSegment(std::string const & name) const
{
    for (auto & segment : segments)
    {
        if (segment.Name() == name)
        {
            return true;
        }
    }
    return false;
}

bool ObjectCode::HaveSegment(SegmentID id) const
{
    for (auto & segment : segments)
    {
        if (segment.ID() == id)
        {
            return true;
        }
    }
    return false;
}

CodeSegment<ObjectCode::AddressType> const & ObjectCode::GetSegment(std::string const & name) const
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

CodeSegment<ObjectCode::AddressType> & ObjectCode::GetSegment(std::string const & name)
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

CodeSegment<ObjectCode::AddressType> const & ObjectCode::GetSegment(SegmentID id) const
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

CodeSegment<ObjectCode::AddressType> & ObjectCode::GetSegment(SegmentID id)
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

std::vector<CodeSegment<ObjectCode::AddressType>> const & ObjectCode::GetSegments() const
{
    return segments;
}

} // namespace ObjectFile
