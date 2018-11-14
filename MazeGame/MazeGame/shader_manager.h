#pragma once
#include <string>
#include "glad.h"

class ShaderManager {
   public:
    static int InitShader(const std::string& vertex_shader_file, const std::string& fragment_shader_file);

    static GLuint textured_shader_;

   private:
    static GLuint CompileShaderProgram(const std::string& vertex_shader_file, const std::string& fragment_shader_file);
    static char* ReadShaderSource(const char* shaderFile);
    static void VerifyShaderCompiled(GLuint shader);
};
