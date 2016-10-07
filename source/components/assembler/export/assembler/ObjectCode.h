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

using SegmentData = std::vector<uint8_t>;

class CodeSegment
{
public:
    CodeSegment(SegmentID id, std::string const & name);
    virtual ~CodeSegment();

    SegmentID ID() const { return id; }
    std::string const & Name() const { return name; }
    uint16_t Offset() const { return offset; }
    uint16_t Size() const { return size; }
    SegmentData const & Data() const { return data; }
    SegmentData & Data() { return data; }

    void SetOffset(uint16_t value) { offset = value; }
    void SetData(SegmentData const & value) { data = value; size = uint16_t(value.size()); }

private:
    SegmentID id;
    std::string name;
    uint16_t offset;
    uint16_t size;
    SegmentData data;
}; // CodeSegment

class ObjectCode
{
public:
    ObjectCode(std::string const & moduleName);
    virtual ~ObjectCode();

    void Clear();

    std::string const & ModuleName() const { return moduleName; }
    void AddSegment(SegmentID id, std::string const & name);
    bool HaveSegment(std::string const & name) const;
    bool HaveSegment(SegmentID id) const;
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
