//
// Created by hugom on 22/06/2024.
//
#include <iostream>
#include "Chunk/Chunk.h"
#include "glm/gtx/string_cast.hpp"

namespace HC {
    void Chunk::Initialize() {
        for (int i = 0; i < CHUNK_SIZE_CUBED; i++) {
            if(GET_COORDS_FROM_INDEX(i).y < 25) {
                blocks[i] = 1;
            }
            else {
                blocks[i] = 0;
            }
        }
        GenerateVerticesAndIndices();
        model = std::make_unique<Model>(vec, my_indices);
    }

    void Chunk::Draw() {
        if (bIsDirty) {
            GenerateVerticesAndIndices();
            model = std::make_unique<Model>(vec, my_indices);
            bIsDirty = false;
        }
        model->Draw();

    }

    Chunk::~Chunk() {

    }

    Chunk::Chunk(const glm::ivec2& position, const ChunkManager& chunkManager) : position(position), chunkManager(chunkManager){

    }

    void Chunk::GenerateVerticesAndIndices() {
        int number_of_faces_rendered = 0;
        vec.clear();
        my_indices.clear();
        for (int i = 0; i < CHUNK_SIZE_CUBED; i++) {
            glm::vec3 coords = GET_COORDS_FROM_INDEX(i);
            if (blocks[i] != 0) {  // Only process non-zero blocks
                // Front face
                if (GetBlockAt(coords.x, coords.y, coords.z + 1) == 0) {
                    for (auto vertex : front_face_vertices) {
                        Vertex v = vertex;  // Make a copy of the vertex
                        v.position += coords ;  // Adjust position
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }

                // Back face
                if (GetBlockAt(coords.x, coords.y, coords.z - 1) == 0) {
                    for (auto vertex : back_face_vertices) {
                        Vertex v = vertex;
                        v.position += coords;
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }

                // Left face
                if (GetBlockAt(coords.x - 1, coords.y, coords.z) == 0) {
                    for (auto vertex : left_face_vertices) {
                        Vertex v = vertex;
                        v.position += coords;
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }

                // Right face
                if (GetBlockAt(coords.x + 1, coords.y, coords.z) == 0) {
                    for (auto vertex : right_face_vertices) {
                        Vertex v = vertex;
                        v.position += coords;
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }

                // Top face
                if (GetBlockAt(coords.x, coords.y + 1, coords.z) == 0) {
                    for (auto vertex : top_face_vertices) {
                        Vertex v = vertex;
                        v.position += coords;
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }

                // Bottom face
                if (GetBlockAt(coords.x, coords.y - 1, coords.z) == 0) {
                    for (auto vertex : bottom_face_vertices) {
                        Vertex v = vertex;
                        v.position += coords;
                        v.position += (glm::ivec3(position.x, 0, position.y) * CHUNK_SIZE);
                        vec.push_back(v);
                    }
                    for (auto indice : block_indices) {
                        my_indices.push_back(indice + number_of_faces_rendered*4);
                    }
                    number_of_faces_rendered++;
                }
            }
        }


    }

    Block Chunk::GetBlockAt(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE_HEIGHT || z < 0 || z >= CHUNK_SIZE) {
            return 0;
        }
        return blocks[GET_CUBE(x, y, z)];
    }

    void Chunk::SetBlockAt(int x, int y, int z, Block block) {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE_HEIGHT || z < 0 || z >= CHUNK_SIZE) {
            return;
        }
        bIsDirty = true;
        blocks[GET_CUBE(x, y, z)] = block;
    }


} // HC