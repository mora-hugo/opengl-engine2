//

#include "Program.h"
#include "glm/gtc/type_ptr.hpp"

namespace HC {
    Program::Program() {
        Id = glCreateProgram();
    }

    Program::~Program() {
        glDeleteProgram(Id);
    }

    void Program::AttachShader(GLuint shaderId) {
        glAttachShader(Id, shaderId);
    }

    void Program::Link() {
        glLinkProgram(Id);
    }

    void Program::GetCompileStatus(CompileStatus &compileStatus) {
        glGetProgramiv(Id, GL_LINK_STATUS, &compileStatus.success);
        if (!compileStatus.success) {
            char infoLog[512];
            glGetProgramInfoLog(Id, 512, NULL, infoLog);
            compileStatus.infoLog = infoLog;
        }

    }

    void Program::DeleteShader(GLuint shaderId) {
        glDeleteShader(shaderId);
    }

    void Program::Use() {
        glUseProgram(Id);
    }

    void Program::SetUniformBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), (int)value);
    }

    void Program::SetUniformInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(Id, name.c_str()), value);
    }

    void Program::SetUniformFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(Id, name.c_str()), value);
    }

    void Program::SetUniformMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
} // HC