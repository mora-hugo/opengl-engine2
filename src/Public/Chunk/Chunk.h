#pragma once

#include <cstdint>
#include <array>
#include <map>
#include "Model.h"
#include "ChunkManager.h"
#define CHUNK_SIZE 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE_HEIGHT * CHUNK_SIZE)
#define GET_CUBE(x, y, z) ((x) + (z) * CHUNK_SIZE + (y) * (CHUNK_SIZE*CHUNK_SIZE))
#define GET_COORDS_FROM_INDEX(index) glm::ivec3((index) % CHUNK_SIZE, (index) / (CHUNK_SIZE*CHUNK_SIZE), ((index) / CHUNK_SIZE) % CHUNK_SIZE)

using Block = uint16_t;
namespace HC {
    class ChunkManager;
    class Chunk {
    public:
        Chunk(const glm::ivec2& position, const ChunkManager& chunkManager);
        ~Chunk();
        Chunk(const Chunk&) = delete;
        Chunk &operator=(const Chunk&) = delete;

        void Initialize();
        void Draw();
        [[nodiscard]] Block GetBlockAt(int x, int y, int z) const;
        void SetBlockAt(int x, int y, int z, Block block);

    protected:
        std::array<Block, CHUNK_SIZE_CUBED> blocks;
        std::map<size_t, Block> blockModified;
        glm::ivec2 position;

    private:
        const ChunkManager& chunkManager;
        std::vector<Vertex> vec;
        std::vector<GLuint> my_indices;
        std::unique_ptr<Model> model;
        bool bIsDirty = false;
    private:
        void GenerateVerticesAndIndices();
    public:
        constexpr static inline glm::vec2 texture_offset = {1.f/24, 1.f/34};
        constexpr static inline std::array<Vertex, 4> front_face_vertices = {
                // Front face
                Vertex{{1.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], texture_offset[1]}},
                Vertex{{1.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
                Vertex{{0.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{0.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
        };

        constexpr static inline std::array<Vertex, 4> back_face_vertices = {
                // Back face
                Vertex{{0.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
                Vertex{{1.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{1.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
                Vertex{{0.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0],texture_offset[1]}},
        };

        constexpr static inline std::array<Vertex, 4> left_face_vertices = {
                // Left face
                Vertex{{0.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{0.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
                Vertex{{0.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], texture_offset[1]}},
                Vertex{{0.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
        };

        constexpr static inline std::array<Vertex, 4> right_face_vertices = {
                // Right face
                Vertex{{1.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
                Vertex{{1.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{1.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
                Vertex{{1.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], texture_offset[1]}},
        };

        constexpr static inline std::array<Vertex, 4> top_face_vertices = {
                // Top face
                Vertex{{0.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
                Vertex{{1.0f,  1.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
                Vertex{{1.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], texture_offset[1]}},
                Vertex{{0.0f,  1.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
        };

        constexpr static inline std::array<Vertex, 4> bottom_face_vertices = {
                // Bottom face
                Vertex{{0.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {0.f, texture_offset[1]}},
                Vertex{{1.0f,  0.0f,  1.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], texture_offset[1]}},
                Vertex{{1.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {texture_offset[0], 0.f}},
                Vertex{{0.0f,  0.0f,  0.0f}, {1.f, 1.f, 1.f}, {0.f, 0.f}},
        };

        constexpr static inline std::array<GLuint, 6> block_indices = {
                // Front face
                0, 1, 2,
                2, 3, 0,
        };

    };

}
