#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "Chunk.h"
using Block = uint8_t;
namespace HC {
    class Chunk;
    class ChunkManager {
    friend class Chunk;
    public:
        ChunkManager() {};
        void Initialize();
        void Draw();
        bool ChunkRayCast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, glm::vec3 & outHitPos, glm::vec3 & outHitNormal);
        uint32_t GetChunkCount() const { return chunks.size(); }
        void SetBlockAt(const glm::ivec3& position, Block block);

        [[nodiscard]] Block GetWorldBlockAt(int x, int y, int z) const;

    protected:



    private:

        std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>> chunks;
    };

}


