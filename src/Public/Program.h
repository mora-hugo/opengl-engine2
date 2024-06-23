#pragma once

#include "glad.h"
#include "Shader.h"
#include "glm/fwd.hpp"

namespace HC {

    class Program {
    public:
            Program();
            ~Program();
            void AttachShader(GLuint shaderId);
            void DeleteShader(GLuint shaderId);
            void Use();
            void Link();
            void GetCompileStatus(CompileStatus& compileStatus);
            GLuint GetId() const { return Id;}
            /* Uniforms */
            void SetUniformBool(const std::string &name, bool value) const;
            void SetUniformInt(const std::string &name, int value) const;
            void SetUniformFloat(const std::string &name, float value) const;
            void SetUniformMat4(const std::string &name, const glm::mat4 &mat) const;



    private:
        GLuint Id;
    };

} // HC

