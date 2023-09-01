#include <iostream>
#include <string>

#include "Shared_Memory_Copy_Tool.h"
#include "Reader.h"
#include "Writer.h"

#include <boost/process.hpp>

int main()
{
    try
    {
        ShMemCopyTool::CopyTool copytool("source.txt", "target.txt", "shMemory");
        if (copytool.isMemFree())
            copytool.readFromFile();
        else
            copytool.writeToFile();
    }
    catch (const std::system_error& err)
    {
        std::cerr << err.what() << '\n';
    }
}
