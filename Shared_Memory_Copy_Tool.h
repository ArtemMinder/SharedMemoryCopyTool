#pragma once

#include <atomic>
#include <string>
#include "Reader.h"
#include "Writer.h"

namespace ShMemCopyTool
{
    class CopyTool
    {
    public:
        CopyTool() = delete;
        CopyTool(std::string sourceFile, std::string targetFile, std::string shMemory);
        CopyTool(const CopyTool&) = delete;
        CopyTool(CopyTool&&) = delete;
        CopyTool& operator=(const CopyTool&) = delete;
        CopyTool& operator=(CopyTool&&) = delete;

        ~CopyTool() = default;

        bool isMemFree() const;
        void readFromFile();
        void writeToFile();

    private:
        std::string _sourceFile;
        std::string _targetFile;
        std::string _shMemory;
    };
}