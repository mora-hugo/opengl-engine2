#pragma once

#include <vector>
#include <string>

namespace HC {

    struct Utils {
        static std::vector<char> ReadFile(const std::string &filepath);
    };


} // HC

