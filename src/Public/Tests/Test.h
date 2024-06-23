#pragma once

#include <iostream>
#include "glm/vec3.hpp"
#include "Chunk/Chunk.h"

class Test {
public:
    static void testIndices() {
        int x = 1, y = 2, z = 3;
        int index = GET_CUBE(x, y, z);
        glm::vec3 coords = GET_COORDS_FROM_INDEX(index);

        std::cout << "Original coords: (" << x << ", " << y << ", " << z << ")\n";
        std::cout << "Calculated index: " << index << "\n";
        std::cout << "Converted coords: (" << coords.x << ", " << coords.y << ", " << coords.z << ")\n";

        if (x == coords.x && y == coords.y && z == coords.z) {
            std::cout << "Test passed!\n";
        } else {
            std::cout << "Test failed!\n";
        }
    }

};

