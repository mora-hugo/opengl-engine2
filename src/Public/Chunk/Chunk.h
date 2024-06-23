#pragma once

#include <cstdint>
#include <array>
#include "Model.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZE_SQUARED (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)
#define GET_CUBE(x, y, z) ((x) + (y) * CHUNK_SIZE + (z) * CHUNK_SIZE_SQUARED)
#define GET_COORDS_FROM_INDEX(index) glm::ivec3(index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARED)

using Block = uint8_t;

namespace HC {

    class Chunk {
    public:
        Chunk();
        ~Chunk();
        Chunk(const Chunk&) = delete;
        Chunk &operator=(const Chunk&) = delete;

        void Initialize();
        void Draw();
        [[nodiscard]] Block GetBlockAt(int x, int y, int z) const;


    protected:
        std::array<Block, CHUNK_SIZE_CUBED> blocks;

        std::vector<Vertex> vec;
        std::vector<GLuint> my_indices;
        std::unique_ptr<Model> model;

    private:
        void GenerateVerticesAndIndices();
    public:
        const static inline std::array<Vertex, 4> front_face_vertices = {
                // Front face
                Vertex{{0.5f,  0.5f,  0.5f}, {0.f, 0.f, 0.f}, {1.f, 1.f}},

                Vertex{{0.5f,  -0.5f, 0.5f}, {0.f, 0.f, 0.f}, {1.f, 0.f}},

                Vertex{{-0.5f, -0.5f, 0.5f}, {0.f, 0.f, 0.f}, {0.f, 0.f}},
                Vertex{{-0.5f, 0.5f,  0.5f}, {0.f, 0.f, 0.f}, {0.f, 1.f}},




        };

        const static inline std::array<Vertex, 4> back_face_vertices = {
                // Back face
                Vertex{{-0.5f, -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {1.f, 0.f}},
                Vertex{{0.5f,  -0.5f, -0.5f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{0.5f,  0.5f,  -0.5f}, {1.f, 1.f, 1.f}, {0.f, 1.f}},
                Vertex{{-0.5f, 0.5f,  -0.5f}, {1.f, 1.f, 1.f}, {1.f, 1.f}},
        };

        const static inline std::array<Vertex, 4> left_face_vertices = {
                // Left face
                Vertex{{-0.5f, -0.5f, -0.5f}, {0.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{-0.5f, 0.5f,  -0.5f}, {0.f, 1.f, 1.f}, {0.f, 1.f}},
                Vertex{{-0.5f, 0.5f,  0.5f},  {0.f, 1.f, 1.f}, {1.f, 1.f}},
                Vertex{{-0.5f, -0.5f, 0.5f},  {0.f, 1.f, 1.f}, {1.f, 0.f}},





        };

        const static inline std::array<Vertex, 4> right_face_vertices = {
                // Right face
                Vertex{{0.5f, -0.5f, -0.5f}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
                Vertex{{0.5f, -0.5f, 0.5f},  {0.f, 0.f, 1.f}, {0.f, 0.f}},
                Vertex{{0.5f, 0.5f,  0.5f},  {0.f, 0.f, 1.f}, {0.f, 1.f}},
                Vertex{{0.5f, 0.5f,  -0.5f}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
        };
        const static inline std::array<Vertex, 4> top_face_vertices = {
                // Top face
                Vertex{{-0.5f, 0.5f, -0.5f}, {1.f, 1.f, 0.f}, {0.f, 0.f}},
                Vertex {{0.5f,  0.5f, -0.5f}, {1.f, 1.f, 0.f}, {1.f, 0.f}},
                Vertex{{0.5f,  0.5f, 0.5f},  {1.f, 1.f, 0.f}, {1.f, 1.f}},
                Vertex{{-0.5f, 0.5f, 0.5f},  {1.f, 1.f, 0.f}, {0.f, 1.f}},
        };
        const static inline std::array<Vertex, 4> bottom_face_vertices = {
                // Bottom face
                Vertex{{-0.5f, -0.5f, 0.5f},  {1.f, 0.f, 0.f}, {0.f, 1.f}},
                Vertex{{0.5f,  -0.5f, 0.5f},  {1.f, 0.f, 0.f}, {1.f, 1.f}},
                Vertex{{0.5f,  -0.5f, -0.5f}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
                Vertex{{-0.5f, -0.5f, -0.5f}, {1.f, 0.f, 0.f}, {0.f, 0.f}},




        };

        const static inline std::array<GLuint, 6> block_indices = {
                // Front face
                0, 1, 2,
                2, 3, 0,
        };

    };

}
