#include "Shared_Memory_Copy_Tool.h"
#include <stdio.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/serialization/string.hpp>
#include <iostream>
#include <string>
#include <thread>

static constexpr size_t BUFF_SIZE = 1048576;

struct SharedMemoryData
{
    std::string buff;
    bool        fin;
    SharedMemoryData() : fin(false) {}
    SharedMemoryData(char* buff, bool isFinish) : buff(buff), fin(isFinish) {}

    inline bool isFinished() const { return fin; }

    friend class boost::serialization::access;

private:
    template <class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        (version);
        archive& fin;
        archive& buff;
    }
};

namespace ShMemCopyTool
{
    using boost::interprocess::create_only;
    using boost::interprocess::open_only;
    using boost::interprocess::read_only;
    using boost::interprocess::read_write;

    CopyTool::CopyTool(
        std::string sourceFile, std::string targetFile, std::string shMemory)
        : _sourceFile(sourceFile),
        _targetFile(targetFile),
        _shMemory(shMemory)
    {
    }

    bool CopyTool::isMemFree() const
    {
        try
        {
            boost::interprocess::shared_memory_object obj{ create_only, _shMemory.c_str(), read_write };
            return true;
        }
        catch (boost::interprocess::interprocess_exception&)
        {
            boost::interprocess::shared_memory_object::remove(_shMemory.c_str());
        }

        return false;
    }

    void CopyTool::readFromFile()
    {
        boost::interprocess::message_queue::remove(_shMemory.c_str());

        constexpr boost::ulong_long_type   MAX_MESSAGES = 100000;
        boost::interprocess::message_queue msgQ(
            create_only, _shMemory.c_str(), MAX_MESSAGES, BUFF_SIZE + sizeof(SharedMemoryData));

        Reader _fileReader;
        _fileReader.open(_sourceFile);

        while (!_fileReader.isEndOfFile())
        {
            const auto       bytesChunk = _fileReader.read(BUFF_SIZE);
            SharedMemoryData shMemBuf{ bytesChunk, _fileReader.isEndOfFile() };

            std::stringstream             ostream;
            boost::archive::text_oarchive oarchive(ostream);
            oarchive << shMemBuf;
            std::string serialized_string(ostream.str());

            msgQ.send(serialized_string.data(), serialized_string.size(), 0);
        }
    }

    void CopyTool::writeToFile()
    {
        boost::interprocess::message_queue msgQ(open_only, _shMemory.c_str());
        boost::ulong_long_type             recievedSize = 0;
        unsigned int                       priority = 0;

        Writer _fileWriter;
        _fileWriter.open(_targetFile);
        SharedMemoryData shMemBuffer;
        while (!shMemBuffer.isFinished())
        {
            std::stringstream istream;
            std::string       serialized_string;
            serialized_string.resize(BUFF_SIZE + sizeof(SharedMemoryData));
            msgQ.receive(&serialized_string[0], BUFF_SIZE + sizeof(SharedMemoryData), recievedSize,
                priority);
            istream << serialized_string;
            boost::archive::text_iarchive iarchive(istream);
            iarchive >> shMemBuffer;

            _fileWriter.write(shMemBuffer.buff.c_str());
        }

        boost::interprocess::message_queue::remove(_shMemory.c_str());
    }

} 