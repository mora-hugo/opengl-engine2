#pragma once

#include <memory>
#include "Shader.h"

namespace HC {

    class Shaders {
    public:
        Shaders(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
        Shaders(const Shaders&) = delete;
        Shaders &operator=(const Shaders&) = delete;

        [[nodiscard]] GLuint GetVertexShaderId() const { return vertexShader->GetId(); }
        [[nodiscard]] GLuint GetFragmentShaderId() const { return fragmentShader->GetId(); }

    private:
        std::unique_ptr<Shader> vertexShader;
        std::unique_ptr<Shader> fragmentShader;
    };

} // HC


