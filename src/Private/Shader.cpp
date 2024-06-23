//
// Created by hugom on 26/12/2023.
//

#include <fstream>
#include <sstream>
#include "Shader.h"

namespace HC {
    Shader::Shader(GLenum shaderType) {
        ShaderType = shaderType;
        Id = glCreateShader(shaderType);
    }

    Shader::~Shader() {

    }

    void Shader::SetSource(const char *source) {
        glShaderSource(Id, 1, &source, NULL);
    }

    void Shader::Compile() {
        glCompileShader(Id);
    }

    void Shader::GetCompileStatus(CompileStatus &compileStatus) {
        glGetShaderiv(Id, GL_COMPILE_STATUS, &compileStatus.success);
        if(!compileStatus.success) {
            char infoLog[512];
            glGetShaderInfoLog(Id, 512, nullptr, infoLog);
            compileStatus.infoLog = std::string(infoLog);
        }

    }

    void Shader::ReadShaderFile(std::string& output, const std::string &filepath) {
        std::ifstream shaderFile;

        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            shaderFile.open(filepath);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();

            // close file handlers
            shaderFile.close();
            // convert stream into string
            output = shaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
        }
    }

    void Shader::SetSourceFromFile(const std::string &filepath) {
        std::string shaderSource;
        ReadShaderFile(shaderSource, filepath);
        SetSource(shaderSource.c_str());
    }


} // HC