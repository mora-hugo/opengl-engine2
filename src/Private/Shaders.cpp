//
// Created by hugom on 28/12/2023.
//

#include <stdexcept>
#include "Shaders.h"

namespace HC {
    Shaders::Shaders(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
        /* Vertex Shader */
        vertexShader = std::make_unique<Shader>(GL_VERTEX_SHADER);
        vertexShader->SetSourceFromFile(vertexShaderPath);
        vertexShader->Compile();

        // check for shader compile errors
        CompileStatus compileStatus;
        vertexShader->GetCompileStatus(compileStatus);
        if (!compileStatus.success) {
            throw std::runtime_error("Failed to compile vertex shader");
        }


        /* Fragment Shader */
        fragmentShader = std::make_unique<Shader>(GL_FRAGMENT_SHADER);
        fragmentShader->SetSourceFromFile(fragmentShaderPath);
        fragmentShader->Compile();

        fragmentShader->GetCompileStatus(compileStatus);

        if (!compileStatus.success) {
            throw std::runtime_error("Failed to compile fragment shader");
        }
    }
} // HC