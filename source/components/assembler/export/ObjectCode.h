#pragma once

#include <string>
#include <vector>

namespace Assembler
{

enum class SegmentID : uint8_t
{
    ASEG = 0,
    CSEG = 1,
    DSEG = 2,
    SSEG = 3,
    Memory = 4,
    Reserved = 5,
};

class CodeSegment
{
public:
    CodeSegment(SegmentID id, std::string const & name);
    virtual ~CodeSegment();

    SegmentID ID() const { return id; }
    std::string const & Name() const { return name; }
    uint16_t Offset() const { return offset; }
    uint16_t Size() const { return size; }
    std::vector<uint8_t> const & Data() const { return data; }

    void SetOffset(uint16_t value) { offset = value; }
    void SetData(std::vector<uint8_t> const & value) { data = value; size = uint16_t(value.size()); }

private:
    SegmentID id;
    std::string name;
    uint16_t offset;
    uint16_t size;
    std::vector<uint8_t> data;
}; // CodeSegment

class ObjectCode
{
public:
    ObjectCode(std::string const & moduleName);
    virtual ~ObjectCode();

    std::string const & ModuleName() const { return moduleName; }
    void AddSegment(SegmentID id, std::string const & name);
    CodeSegment const & GetSegment(std::string const & name) const;
    CodeSegment & GetSegment(std::string const & name);
    CodeSegment const & GetSegment(SegmentID id) const;
    CodeSegment & GetSegment(SegmentID id);
    std::vector<CodeSegment> const & GetSegments() const;

private:
    std::string moduleName;
    std::vector<CodeSegment> segments;
    std::vector<std::string> externalNames;
}; // ObjectCode

} // namespace Assembler
