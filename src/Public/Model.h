#pragma once
#include <vector>
#include <memory>
#include "glad.h"
#include "glm/glm.hpp"
#include "stb_image.h"


namespace HC {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    struct TextureData {
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char* data = nullptr;

        void Free() {
            if(data) {
                stbi_image_free(data);
            }
        }

    };

    class Model {
    public:
        Model(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::shared_ptr<TextureData>& textureDataObject = nullptr);

        ~Model();

        void Draw();
        Model(const Model&) = delete;
        Model &operator=(const Model&) = delete;

        [[nodiscard]] GLuint GetVAO() const { return VAO; }
        [[nodiscard]] GLuint GetVBO() const { return VBO; }
        [[nodiscard]] GLuint GetEBO() const { return EBO; }

        /* Textures */
        static std::shared_ptr<TextureData> LoadTexture(const std::string& filepath);
        static void FreeTextureData(TextureData* textureData);

        static GLuint CREATE_TEXTURE(const std::shared_ptr<TextureData>& textureDataObject);
        static void BIND_TEXTURE(GLuint textureId);
    protected:
        /* Textures */
        void CreateTexture(const std::shared_ptr<TextureData>& textureDataObject);
        void BindTexture();
    private:
        /* Buffers */
        void CreateBuffers();
        void BindBuffers(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);




    private:
        size_t vertexCount = 0;
        size_t indexCount = 0;
        bool bUsingIndexBuffer = false;

        GLuint VBO, VAO, EBO;

        GLuint texture = 0;

    };

} // HC

