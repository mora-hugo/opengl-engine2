#pragma once
#include <fstream>
#include <cstdint>
#include "windows.h"

namespace HC {
    class File {
    public:
        File(const std::string& path);
        ~File() = default;
        virtual bool IsOpen() = 0;

        size_t GetFileContentSize() const;

        virtual void CloseFile() = 0;
        inline std::string GetFilePath() const {return filepath;}
    protected:
        virtual void OpenFile() = 0;
    protected:
        std::string filepath;

    };

    class FileReader : public File {
    public:
        FileReader(const std::string& path);
        void OpenFile() override;
        void CloseFile() override;
        void Read(uint8_t *data, size_t size);
        bool IsOpen() override;

    private:
        std::ifstream file;
    };

    class FileWriter : public File {
    public:
        FileWriter(const std::string& path);

        void OpenFile() override;
        void CloseFile() override;
        void Write(const uint8_t* data, size_t size);
        bool IsOpen() override;
    private:
        std::ofstream file;
    };
}



