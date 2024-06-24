//
// Created by hugom on 23/06/2024.
//
#include "Chunk/Chunk.h"
#include "Chunk/ChunkManager.h"
//allow glm serialization


void HC::ChunkManager::Draw() {
    for (auto& chunk : chunks) {
        chunk.second->Draw();
    }

}

void HC::ChunkManager::Initialize() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {

            std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>(glm::ivec2(i, j), *this);
            chunk->Initialize();
            chunks[glm::ivec2(i, j)] = std::move(chunk);
        }
    }
}

Block HC::ChunkManager::GetWorldBlockAt(int x, int y, int z) const {
    // Ensure correct chunk index calculation for negative coordinates
    int chunk_x = (x >= 0) ? x / CHUNK_SIZE : (x - (CHUNK_SIZE - 1)) / CHUNK_SIZE;
    int chunk_z = (z >= 0) ? z / CHUNK_SIZE : (z - (CHUNK_SIZE - 1)) / CHUNK_SIZE;

    auto chunk = chunks.find(glm::ivec2(chunk_x, chunk_z));
    if (chunk == chunks.end()) {
        return 0; // No chunk found, return empty block
    } else {
        // Calculate local block coordinates within the chunk
        int local_x = x - chunk_x * CHUNK_SIZE;
        int local_z = z - chunk_z * CHUNK_SIZE;
        return chunk->second->GetBlockAt(local_x, y, local_z);
    }
}

bool HC::ChunkManager::ChunkRayCast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, glm::vec3 & outHitPos, glm::vec3 & outHitNormal) {

    //Bresenham's line algorithm
    glm::vec3 end = origin + direction * maxDistance;

    glm::ivec3 startPos = glm::floor(origin);
    glm::ivec3 endPos = glm::floor(end);

    glm::ivec3 directionSign = glm::sign(direction);
    glm::vec3 deltaDist = glm::abs(glm::vec3(1.0f) / direction);
    glm::vec3 sideDist;

    if (directionSign.x == 1)
        sideDist.x = (startPos.x + 1.0f - origin.x) * deltaDist.x;
    else
        sideDist.x = (origin.x - startPos.x) * deltaDist.x;

    if (directionSign.y == 1)
        sideDist.y = (startPos.y + 1.0f - origin.y) * deltaDist.y;
    else
        sideDist.y = (origin.y - startPos.y) * deltaDist.y;

    if (directionSign.z == 1)
        sideDist.z = (startPos.z + 1.0f - origin.z) * deltaDist.z;
    else
        sideDist.z = (origin.z - startPos.z) * deltaDist.z;

    glm::ivec3 currentPos = startPos;
    glm::vec3 hitNormal{0.f};
    while (glm::length(glm::vec3(currentPos) - origin) < maxDistance) {
        if (GetWorldBlockAt(currentPos.x, currentPos.y, currentPos.z) != 0) {
            outHitPos = currentPos;
            outHitNormal = hitNormal;
            return true;
        }

        if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
            sideDist.x += deltaDist.x;
            currentPos.x += directionSign.x;
            hitNormal = glm::vec3(-directionSign.x, 0, 0);
        } else if (sideDist.y < sideDist.z) {
            sideDist.y += deltaDist.y;
            currentPos.y += directionSign.y;
            hitNormal = glm::vec3(0, -directionSign.y, 0);
        } else {
            sideDist.z += deltaDist.z;
            currentPos.z += directionSign.z;
            hitNormal = glm::vec3(0, 0, -directionSign.z);
        }
    }

    return false; // Si aucun bloc n'est touché, retourner la position de fin
}

void HC::ChunkManager::SetBlockAt(const glm::ivec3 &position, Block block) {
// Ensure correct chunk index calculation for negative coordinates
    int chunk_x = (position.x >= 0) ? position.x / CHUNK_SIZE : (position.x - (CHUNK_SIZE - 1)) / CHUNK_SIZE;
    int chunk_z = (position.z >= 0) ? position.z / CHUNK_SIZE : (position.z - (CHUNK_SIZE - 1)) / CHUNK_SIZE;

    auto chunk = chunks.find(glm::ivec2(chunk_x, chunk_z));
    if (chunk == chunks.end()) {
        return;
    } else {
        // Calculate local block coordinates within the chunk
        int local_x = position.x - chunk_x * CHUNK_SIZE;
        int local_z = position.z - chunk_z * CHUNK_SIZE;
        return chunk->second->SetBlockAt(local_x, position.y, local_z, block);
    }

}





