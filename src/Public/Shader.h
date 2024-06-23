#pragma once

#include <string>
#include "glad.h"

namespace HC {
    struct CompileStatus {
        int success;
        std::string infoLog;
    };
    class Shader {
    public:
        Shader(GLenum shaderType);
        ~Shader();
        void SetSource(const char *source);
        void SetSourceFromFile(const std::string &filepath);
        void Compile();
        void GetCompileStatus(CompileStatus& compileStatus);
        GLuint GetId() const { return Id;}
    private:
        GLuint Id;
        GLenum ShaderType;

        void ReadShaderFile(std::string& output, const std::string &filepath);
    };

} // HC

