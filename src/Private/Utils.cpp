//
// Created by hugom on 26/12/2023.
//

#include <fstream>
#include "Utils.h"

namespace HC {
    std::vector<char> Utils::ReadFile(const std::string &filepath) {
        // std::ios::ate make the file cursor to the end
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if(!file.is_open()) {
            throw(std::runtime_error("failed to open file : " + filepath));
        }
        // file.tellg() give the cursor position, if the cursor is at the end, this is the file size
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }
} // HC