#include "object-file/ObjectFile.h"

#include <fstream>

namespace ObjectFile
{

enum class RecordType : uint8_t
{
    Undefined = 0x00,
    ModuleHeader = 0x02,
    ModuleEnd = 0x04,
    Content = 0x06,
    LineNumbers = 0x08,
    EndOfFile = 0x0E,
    ModuleAncestor = 0x10,
    LocalSymbols = 0x12,
    PublicDeclarations = 0x16,
    ExternalNames = 0x18,
    ExternalReferences = 0x20,
    Relocations = 0x22,
    InterSegmentReferences = 0x24,
    LibraryModuleLocations = 0x26,
    LibraryModuleNames = 0x28,
    LibraryDictionary = 0x2A,
    LibraryHeader = 0x2C,
    NamedCommonDefinitions = 0x2E,
};

enum class AlignmentType : uint8_t
{
    InpageRelocatable = 0x01,
    PageRelocatable = 0x02,
    ByteRelocatable = 0x03,
};

class Record
{
public:
    Record()
        : recordType(RecordType::Undefined)
        , recordLength(1)
        , recordData()
        , recordChecksum()
    {}
    Record(RecordType recordType)
        : recordType(recordType)
        , recordLength(1)
        , recordData()
        , recordChecksum()
    {}
    Record(RecordType recordType, std::vector<uint8_t> const & recordData)
        : recordType(recordType)
        , recordLength(uint16_t(recordData.size()))
        , recordData(recordData)
        , recordChecksum()
    {}

    void AddToChecksum(uint8_t data)
    {
        recordChecksum = (recordChecksum + data) & 0xFF;
    }
    void CalculateChecksum()
    {
        recordChecksum = 0;
        AddToChecksum(uint8_t(recordType));
        AddToChecksum((recordLength >> 0) & 0xFF);
        AddToChecksum((recordLength >> 8) & 0xFF);
        for (auto data : recordData)
        {
            AddToChecksum(data);
        }
        recordChecksum = ((recordChecksum ^ 0xFF) + 1) & 0xFF;
    }
    void Write(std::ostream & stream)
    {
        recordLength += recordData.size();
        CalculateChecksum();
        stream.write((const char *)&recordType, sizeof(recordType));
        stream.write((const char *)&recordLength, sizeof(recordLength));
        stream.write((const char *)recordData.data(), recordData.size());
        stream.write((const char *)&recordChecksum, sizeof(recordChecksum));
    }
    
protected:
    RecordType recordType;
    uint16_t recordLength;
    std::vector<uint8_t> recordData;
    uint8_t recordChecksum;

    void Append(uint8_t data)
    {
        recordData.push_back(data);
    }
    void Append(uint16_t data)
    {
        recordData.push_back((data >> 0) & 0xFF);
        recordData.push_back((data >> 8) & 0xFF);
    }
    void Append(std::string const & data, uint8_t maxLen = 255)
    {
        uint8_t length = maxLen;
        if (data.length() < length)
            length = uint8_t(data.length());
        Append(length);
        for (uint8_t index = 0; index < length; ++index)
            Append(uint8_t(data[index]));
    }
};

struct Name
{
    uint8_t length;
    std::vector<char> name;
};

struct SegmentDescriptor
{
    SegmentID segmentID;
    uint16_t segmentLength;
    AlignmentType alignmentType;
};

struct NamedCommonDefinitionDescriptor
{
    SegmentID segmentID;
    std::string name;
};

struct SymbolDescriptor
{
    uint16_t offset;
    std::string name;
};

struct ContentDescriptor
{
    SegmentID segmentID;
    uint16_t offset;
    std::vector<uint8_t> data;
};

enum class ReferenceType
{
    Low = 1,
    High = 2,
    Both = 3,
};

struct LocationDescriptor
{
    uint16_t offset;
};

struct ExternalReferenceDescriptor
{
    uint16_t externalNameIndex;
    uint16_t offset;
};

struct LineNumberDescriptor
{
    uint16_t offset;
    uint16_t lineNumber;
};

struct ModuleLocationDescriptor
{
    uint16_t blockNumber;
    uint16_t byteNumber;
};

struct ModuleDictionaryDescriptor
{
    std::vector<std::string> modulePublicNames;
};

//=================================================================================================

class ModuleHeader : public Record
{
public:
    ModuleHeader(std::string const & moduleName, std::vector<SegmentDescriptor> const & segments)
        : Record(RecordType::ModuleHeader)
    {
        Append(moduleName, 31);
        Append(uint8_t{ 0 });
        Append(uint8_t{ 0 });
        for (auto & segment : segments)
        {
            Append(uint8_t(segment.segmentID));
            Append(segment.segmentLength);
            Append(uint8_t(segment.alignmentType));
        }
    }
};

class ModuleEnd : public Record
{
public:
    ModuleEnd(bool moduleIsMain, SegmentID startSegment, uint16_t startOffset)
        : Record(RecordType::ModuleEnd)
    {
        Append(moduleIsMain ? uint8_t{ 0x01 } : uint8_t{ 0x00 });
        Append(uint8_t(startSegment));
        Append(startOffset);
    }
};

class NamedCommonDefinition : public Record
{
public:
    NamedCommonDefinition(std::vector<NamedCommonDefinitionDescriptor> const & names)
        : Record(RecordType::NamedCommonDefinitions)
    {
        for (auto & name : names)
        {
            Append(uint8_t(name.segmentID));
            Append(name.name);
        }
    }
};

class ExternalNames : public Record
{
public:
    ExternalNames(std::vector<std::string> const & names)
        : Record(RecordType::ExternalNames)
    {
        for (auto & name : names)
        {
            Append(name);
            Append(uint8_t{ 0 });
        }
    }
};

class PublicDeclarations : public Record
{
public:
    PublicDeclarations(SegmentID segmentID, std::vector<SymbolDescriptor> const & declarations)
        : Record(RecordType::PublicDeclarations)
    {
        Append(uint8_t(segmentID));
        for (auto & declaration : declarations)
        {
            Append(declaration.offset);
            Append(declaration.name);
            Append(uint8_t{ 0 });
        }
    }
};

class Content : public Record
{
public:
    Content(ContentDescriptor const & content)
        : Record(RecordType::Content)
    {
        Append(uint8_t(content.segmentID));
        Append(content.offset);
        for (auto data : content.data)
            Append(data);
    }
};

class Relocations : public Record
{
public:
    Relocations(ReferenceType referenceType, std::vector<LocationDescriptor> const & relocations)
        : Record(RecordType::Relocations)
    {
        Append(uint8_t(referenceType));
        for (auto & relocation : relocations)
        {
            Append(relocation.offset);
        }
    }
};

class InterSegmentReferences : public Record
{
public:
    InterSegmentReferences(SegmentID segmentID, ReferenceType referenceType, std::vector<LocationDescriptor> const & references)
        : Record(RecordType::InterSegmentReferences)
    {
        Append(uint8_t(segmentID));
        Append(uint8_t(referenceType));
        for (auto & reference : references)
        {
            Append(reference.offset);
        }
    }
};

class ExternalReferences : public Record
{
public:
    ExternalReferences(ReferenceType referenceType, std::vector<ExternalReferenceDescriptor> const & references)
        : Record(RecordType::ExternalReferences)
    {
        Append(uint8_t(referenceType));
        for (auto & reference : references)
        {
            Append(reference.externalNameIndex);
            Append(reference.offset);
        }
    }
};

class ModuleAncestor : public Record
{
public:
    ModuleAncestor(std::string const & moduleName)
        : Record(RecordType::ModuleAncestor)
    {
        Append(moduleName, 31);
    }
};

class LocalSymbols : public Record
{
public:
    LocalSymbols(SegmentID segmentID, std::vector<SymbolDescriptor> const & declarations)
        : Record(RecordType::LocalSymbols)
    {
        Append(uint8_t(segmentID));
        for (auto & declaration : declarations)
        {
            Append(declaration.offset);
            Append(declaration.name);
            Append(uint8_t{ 0 });
        }
    }
};

class LineNumbers : public Record
{
public:
    LineNumbers(SegmentID segmentID, std::vector<LineNumberDescriptor> const & lineNumbers)
        : Record(RecordType::LineNumbers)
    {
        Append(uint8_t(segmentID));
        for (auto & lineNumber : lineNumbers)
        {
            Append(lineNumber.offset);
            Append(lineNumber.lineNumber);
        }
    }
};

class EndOfFile : public Record
{
public:
    EndOfFile()
        : Record(RecordType::EndOfFile)
    {
    }
};

class LibraryHeader : public Record
{
public:
    LibraryHeader(uint16_t moduleCount, uint16_t blockNumber, uint16_t byteNumber)
        : Record(RecordType::LibraryHeader)
    {
        Append(moduleCount);
        Append(blockNumber);
        Append(byteNumber);
    }
};

class LibraryModuleNames : public Record
{
public:
    LibraryModuleNames(std::vector<std::string> const & moduleNames)
        : Record(RecordType::LibraryModuleNames)
    {
        for (auto & moduleName : moduleNames)
        {
            Append(moduleName);
        }
    }
};

class LibraryModuleLocations : public Record
{
public:
    LibraryModuleLocations(std::vector<ModuleLocationDescriptor> const & moduleLocations)
        : Record(RecordType::LibraryModuleLocations)
    {
        for (auto & moduleLocation : moduleLocations)
        {
            Append(moduleLocation.blockNumber);
            Append(moduleLocation.byteNumber);
        }
    }
};

class LibraryDictionary : public Record
{
public:
    LibraryDictionary(std::vector<ModuleDictionaryDescriptor> const & libraryDictionary)
        : Record(RecordType::LibraryDictionary)
    {
        for (auto & moduleDictionary : libraryDictionary)
        {
            for (auto & publicSymbol : moduleDictionary.modulePublicNames)
                Append(publicSymbol);
            Append(uint8_t{ 0 });
        }
    }
};

//=================================================================================================

ObjectFile::ObjectFile()
{
}

ObjectFile::~ObjectFile()
{
}

void ObjectFile::LoadObjectCode(std::string const & path, ObjectCode & code)
{
    LoadObjectCode(std::ifstream(path, std::ios::binary), code);
}

void ObjectFile::LoadObjectCode(std::istream & stream, ObjectCode & code)
{
}

void ObjectFile::WriteObjectCode(std::string const & path, ObjectCode const & code)
{
    WriteObjectCode(std::ofstream(path, std::ios::binary), code);
}

void ObjectFile::WriteObjectCode(std::ostream & stream, ObjectCode const & code)
{
    std::vector<SegmentDescriptor> segments;
    for (auto segment : code.GetSegments())
    {
        segments.push_back(SegmentDescriptor{ segment.ID(), segment.Size(), AlignmentType::ByteRelocatable });
    }
    ModuleHeader moduleHeader(code.ModuleName(), segments);
    moduleHeader.Write(stream);
    for (auto segment : code.GetSegments())
    {
        Content content(ContentDescriptor{ segment.ID(), segment.Offset(), segment.Data() });
        content.Write(stream);
    }
    ModuleEnd moduleEnd(true, SegmentID::CSEG, 0);
    moduleEnd.Write(stream);
    EndOfFile endOfFile;
    endOfFile.Write(stream);
}

} // namespace ObjectFile
