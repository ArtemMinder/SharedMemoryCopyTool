#pragma once
#include <exception>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

class Reader
{
public:
    Reader() = default;
    Reader(const std::string& path)
    {
        open(path);
    }
    Reader(const Reader&) = delete;
    Reader(Reader&&) = delete;
    Reader& operator=(const Reader&) = delete;
    Reader& operator=(Reader&&) = delete;

    ~Reader()
    {
        if (_source)
            _source.close();
    }

    void open(const std::string& path)
    {
        _source.open(path, std::ios_base::binary);
        if (!_source)
            throw std::runtime_error("Source file not found");
    }

    char* read(const size_t bufferSize)
    {
        std::vector<char>buffer(bufferSize, '\0');
        _source.read(buffer.data(), bufferSize);
        const size_t actualRead = _source.gcount();
        buffer.resize(actualRead);
        auto pBuffer = new char[buffer.size() + 1];
        std::copy(buffer.begin(), buffer.end(), pBuffer);
        pBuffer[buffer.size()] = '\0';
        return pBuffer;
    }

    void close()
    {
        _source.close();
    }

    bool isEndOfFile() const
    {
        return _source.eof();
    }

private:
    std::ifstream _source;
};