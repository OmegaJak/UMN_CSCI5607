#include "constants.h"
#include "shader_manager.h"

int ShaderManager::InitShader(const std::string& vertex_shader_file, const std::string& fragment_shader_file) {
    textured_shader_ = CompileShaderProgram(vertex_shader_file, fragment_shader_file);

    return textured_shader_;
}

GLuint ShaderManager::CompileShaderProgram(const std::string& vertex_shader_file, const std::string& fragment_shader_file) {
    GLuint vertex_shader, fragment_shader;
    GLchar *vs_text, *fs_text;
    GLuint program;

    // check GLSL version
    printf("GLSL version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Create shader handlers
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read source code from shader files
    vs_text = ReadShaderSource(vertex_shader_file.c_str());
    fs_text = ReadShaderSource(fragment_shader_file.c_str());

    // error check
    if (vs_text == NULL) {
        printf("Failed to read from vertex shader file %s\n", vertex_shader_file.c_str());
        exit(1);
    } else if (DEBUG_ON) {
        printf("Vertex Shader:\n=====================\n");
        printf("%s\n", vs_text);
        printf("=====================\n\n");
    }
    if (fs_text == NULL) {
        printf("Failed to read from fragment shader file %s\n", fragment_shader_file.c_str());
        exit(1);
    } else if (DEBUG_ON) {
        printf("\nFragment Shader:\n=====================\n");
        printf("%s\n", fs_text);
        printf("=====================\n\n");
    }

    // Load Vertex Shader
    const char* vv = vs_text;
    glShaderSource(vertex_shader, 1, &vv, NULL);  // Read source
    glCompileShader(vertex_shader);               // Compile shaders
    VerifyShaderCompiled(vertex_shader);          // Check for errors

    // Load Fragment Shader
    const char* ff = fs_text;
    glShaderSource(fragment_shader, 1, &ff, NULL);
    glCompileShader(fragment_shader);
    VerifyShaderCompiled(fragment_shader);

    // Create the program
    program = glCreateProgram();

    // Attach shaders to program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // Link and set program to use
    glLinkProgram(program);

    return program;
}

// Create a NULL-terminated string by reading the provided file
char* ShaderManager::ReadShaderSource(const char* shaderFile) {
    FILE* fp;
    long length;
    char* buffer;

    // open the file containing the text of the shader code
    fopen_s(&fp, shaderFile, "r");

    // check for errors in opening the file
    if (fp == NULL) {
        printf("Can't open shader source file %s\n", shaderFile);
        return NULL;
    }

    // determine the file size
    fseek(fp, 0, SEEK_END);  // move position indicator to the end of the file;
    length = ftell(fp);      // return the value of the current position

    // allocate a buffer with the indicated number of bytes, plus one
    buffer = new char[length + 1];

    // read the appropriate number of bytes from the file
    fseek(fp, 0, SEEK_SET);        // move position indicator to the start of the file
    fread(buffer, 1, length, fp);  // read all of the bytes

    // append a NULL character to indicate the end of the string
    buffer[length] = '\0';

    // close the file
    fclose(fp);

    // return the string
    return buffer;
}

void ShaderManager::VerifyShaderCompiled(GLuint shader) {
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printf("Vertex shader failed to compile:\n");
        if (DEBUG_ON) {
            GLint logMaxSize, logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMaxSize);
            printf("printing error message of %d bytes\n", logMaxSize);
            char* logMsg = new char[logMaxSize];
            glGetShaderInfoLog(shader, logMaxSize, &logLength, logMsg);
            printf("%d bytes retrieved\n", logLength);
            printf("error message: %s\n", logMsg);
            delete[] logMsg;
        }
        exit(1);
    }
}

GLuint ShaderManager::textured_shader_;
