#pragma once
#include <fstream>
#include <cstdint>
#include "windows.h"

namespace HC {
    class FileIO {
    public:
        FileIO(const std::string& path);
        ~FileIO();

        void Write(const uint8_t* data, size_t size);
        void Read(const uint8_t* data, size_t size);


        void CloseFile();

    protected:
        void OpenFile(const std::string& path);

    private:
        std::fstream file;

    };
}



