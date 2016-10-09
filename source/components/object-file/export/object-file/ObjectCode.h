#pragma once

#include <string>
#include <vector>

namespace ObjectFile
{

enum class SegmentID : uint8_t
{
    ASEG = 0,
    CSEG = 1,
    DSEG = 2,
    SSEG = 3,
    Memory = 4,
    Reserved = 5,
    Invalid = 255,
};

using SegmentData = std::vector<uint8_t>;

template<class AddressType>
class CodeSegment
{
public:
    CodeSegment(SegmentID id, std::string const & name)
        : id(id)
        , name(name)
        , offset()
        , size()
        , data()
    {
    }
    CodeSegment(CodeSegment const & other)
        : id(other.id)
        , name(other.name)
        , offset(other.offset)
        , size(other.size)
        , data(other.data)
    {
    }
    CodeSegment(CodeSegment && other)
        : id(std::move(other.id))
        , name(std::move(other.name))
        , offset(std::move(other.offset))
        , size(std::move(other.size))
        , data(std::move(other.data))
    {
        other.id = SegmentID::Invalid;
        other.offset = 0;
        other.size = 0;
    }

    virtual ~CodeSegment()
    {
    }
    CodeSegment & operator = (CodeSegment const & other)
    {
        if (this != &other)
        {
            id = other.id;
            name = other.name;
            offset = other.offset;
            size = other.size;
            data = other.data;
        }
        return *this;
    }
    CodeSegment & operator = (CodeSegment && other)
    {
        if (this != &other)
        {
            id = std::move(other.id);
            name = std::move(other.name);
            offset = std::move(other.offset);
            size = std::move(other.size);
            data = std::move(other.data);
            other.id = SegmentID::Invalid;
            other.offset = 0;
            other.size = 0;
        }
        return *this;
    }

    SegmentID ID() const { return id; }
    std::string const & Name() const { return name; }
    AddressType Offset() const { return offset; }
    AddressType Size() const { return size; }
    SegmentData const & Data() const { return data; }
    SegmentData & Data() { return data; }

    void SetOffset(AddressType value) { offset = value; }
    void SetData(SegmentData const & value) { data = value; size = AddressType(value.size()); }

private:
    SegmentID id;
    std::string name;
    AddressType offset;
    AddressType size;
    SegmentData data;
}; // CodeSegment

class ObjectCode
{
public:
    using AddressType = uint16_t;
    ObjectCode(std::string const & moduleName);
    virtual ~ObjectCode();

    void Clear();

    std::string const & ModuleName() const { return moduleName; }
    void AddSegment(SegmentID id, std::string const & name);
    void AddSegment(CodeSegment<AddressType> const & segment);
    void AddSegment(CodeSegment<AddressType> && segment);
    bool HaveSegment(std::string const & name) const;
    bool HaveSegment(SegmentID id) const;
    CodeSegment<AddressType> const & GetSegment(std::string const & name) const;
    CodeSegment<AddressType> & GetSegment(std::string const & name);
    CodeSegment<AddressType> const & GetSegment(SegmentID id) const;
    CodeSegment<AddressType> & GetSegment(SegmentID id);
    std::vector<CodeSegment<AddressType>> const & GetSegments() const;

private:
    std::string moduleName;
    std::vector<CodeSegment<AddressType>> segments;
    std::vector<std::string> externalNames;
}; // ObjectCode

} // namespace ObjectFile
