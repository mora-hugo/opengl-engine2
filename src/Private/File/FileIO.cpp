//
// Created by hugom on 24/06/2024.
//
#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <vector>
#include "File/FileIO.h"

HC::File::File(const std::string &path) : filepath(path)
{
}









size_t HC::File::GetFileContentSize() const {
    if(!std::filesystem::exists(filepath)) {
        std::cerr << "File " << filepath <<" does not exist" << std::endl;
        return 0;
    }
    return std::filesystem::file_size(filepath);

}

HC::FileReader::FileReader(const std::string& path) : File(path) {

}

void HC::FileReader::Read(uint8_t *data, size_t size) {
    file.read(reinterpret_cast<char*>(data), size);
}

void HC::FileReader::OpenFile() {
    try {
        file = std::ifstream(filepath, std::ios::binary);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    if (!IsOpen()) {
        std::cerr << "Failed to open file \""<< filepath << "\"" << std::endl;
    }

}

void HC::FileReader::CloseFile() {
    if(IsOpen())
        file.close();
}

bool HC::FileReader::IsOpen() {
    return file.is_open();
}

HC::FileWriter::FileWriter(const std::string& filepath)  : File(filepath) {

}

void HC::FileWriter::OpenFile() {
    file = std::ofstream(filepath);
    if (!IsOpen()) {
        std::cerr << "Failed to open file" << std::endl;
    }
}

void HC::FileWriter::CloseFile() {
    if(IsOpen())
        file.close();
}


void HC::FileWriter::Write(const uint8_t *data, size_t size) {
    file.write(reinterpret_cast<const char*>(data), size);
}

bool HC::FileWriter::IsOpen() {
    return file.is_open();
}
