//
// Created by hugom on 24/06/2024.
//
#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include "File/FileIO.h"

HC::FileIO::FileIO(const std::string &path)
{
    OpenFile(path);
}


HC::FileIO::~FileIO() {
    CloseFile();
}

void HC::FileIO::OpenFile(const std::string &path) {
    file = std::fstream(path, std::ios::in | std::ios::out | std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }
}

void HC::FileIO::CloseFile() {
    if (file.is_open())
        file.close();

}

void HC::FileIO::Write(const uint8_t *data, size_t size) {
    file.write(reinterpret_cast<const char*>(data), size);
}

void HC::FileIO::Read(const uint8_t *data, size_t size) {
    file.read(reinterpret_cast<char*>(const_cast<uint8_t*>(data)), size);
}








