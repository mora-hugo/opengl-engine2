//
// Created by hugom on 26/12/2023.
//

#include <iostream>
#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace HC {
    Model::Model(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices = {}, const std::shared_ptr<TextureData>& textureDataObject) {
        bUsingIndexBuffer = !indices.empty();
        vertexCount = vertices.size();
        indexCount = indices.size();

        CreateBuffers();

        BindBuffers(vertices, indices);

        if(textureDataObject) {
            CreateTexture(textureDataObject);
        }
    }

    void Model::Draw() {


        glBindVertexArray(VAO);
        if(texture != 0)
            BindTexture();

        if(bUsingIndexBuffer)
            glDrawElements(draw_mode, indexCount, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(draw_mode, 0, vertexCount);


    }

    Model::~Model() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if(bUsingIndexBuffer)
            glDeleteBuffers(1, &EBO);
    }

    void Model::CreateBuffers() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    void Model::BindBuffers(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint) , indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    std::shared_ptr<TextureData> Model::LoadTexture(const std::string &filepath) {
        std::shared_ptr<TextureData> textureData = std::make_unique<TextureData>();
        textureData->data = stbi_load(filepath.c_str(), &textureData->width, &textureData->height, &textureData->channels, 0);
        return std::move(textureData);
    }

    void Model::FreeTextureData(TextureData *textureData) {
        stbi_image_free(textureData->data);
    }

    void Model::CreateTexture(const std::shared_ptr<TextureData>& textureDataObject) {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        /* set the texture wrapping/filtering options (on the currently bound texture object) */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (textureDataObject->data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureDataObject->width, textureDataObject->height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, textureDataObject->data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void Model::BindTexture() {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    GLuint Model::CREATE_TEXTURE(const std::shared_ptr<TextureData> &textureDataObject) {
        GLuint texture;
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        /* set the texture wrapping/filtering options (on the currently bound texture object) */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (textureDataObject->data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureDataObject->width, textureDataObject->height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, textureDataObject->data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        return texture;
    }

    void Model::BIND_TEXTURE(GLuint textureId) {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }


} // HC