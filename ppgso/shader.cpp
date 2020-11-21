#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader.h"


ppgso::Shader::Shader(const ShaderConfig &shaderConfig) {
    // Create shaders
    auto program_id = glCreateProgram();
    auto result = GL_FALSE;
    auto info_length = 0;

    // Compile vertex shader
    auto vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    if (!shaderConfig.vs.empty()) {
        auto vertex_shader_code_ptr = shaderConfig.vs.c_str();
        glShaderSource(vertex_shader_id, 1, &vertex_shader_code_ptr, nullptr);
        glCompileShader(vertex_shader_id);

        // Check vertex shader log
        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_length);
            std::string vertex_shader_log((unsigned int) info_length, ' ');
            glGetShaderInfoLog(vertex_shader_id, info_length, nullptr,
                               &vertex_shader_log[0]);
            std::stringstream msg;
            msg << "Error Compiling Vertex Shader ..." << std::endl;
            msg << vertex_shader_log;
            throw std::runtime_error(msg.str());
        }
        glAttachShader(program_id, vertex_shader_id);
    }

    // Compile tesselation control shader
    auto tcs_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);
    if (!shaderConfig.tcs.empty()) {
        auto code_ptr = shaderConfig.tcs.c_str();
        glShaderSource(tcs_shader_id, 1, &code_ptr, nullptr);
        glCompileShader(tcs_shader_id);

        // Check vertex shader log
        glGetShaderiv(tcs_shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(tcs_shader_id, GL_INFO_LOG_LENGTH, &info_length);
            std::string shader_log((unsigned int) info_length, ' ');
            glGetShaderInfoLog(tcs_shader_id, info_length, nullptr,
                               &shader_log[0]);
            std::stringstream msg;
            msg << "Error Compiling Tesselation Control Shader ..." << std::endl;
            msg << shader_log;
            throw std::runtime_error(msg.str());
        }
        glAttachShader(program_id, tcs_shader_id);
    }

    // Compile tesselation evaluation shader
    auto tes_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
    if (!shaderConfig.tes.empty()) {
        auto code_ptr = shaderConfig.tes.c_str();
        glShaderSource(tes_shader_id, 1, &code_ptr, nullptr);
        glCompileShader(tes_shader_id);

        // Check vertex shader log
        glGetShaderiv(tes_shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(tes_shader_id, GL_INFO_LOG_LENGTH, &info_length);
            std::string shader_log((unsigned int) info_length, ' ');
            glGetShaderInfoLog(tes_shader_id, info_length, nullptr,
                               &shader_log[0]);
            std::stringstream msg;
            msg << "Error Compiling Tesselation Evaluation Shader ..." << std::endl;
            msg << shader_log;
            throw std::runtime_error(msg.str());
        }
        glAttachShader(program_id, tes_shader_id);
    }

    // Compile tesselation evaluation shader
    auto geom_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    if (!shaderConfig.gs.empty()) {
        auto code_ptr = shaderConfig.gs.c_str();
        glShaderSource(geom_shader_id, 1, &code_ptr, nullptr);
        glCompileShader(geom_shader_id);

        // Check vertex shader log
        glGetShaderiv(geom_shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(geom_shader_id, GL_INFO_LOG_LENGTH, &info_length);
            std::string shader_log((unsigned int) info_length, ' ');
            glGetShaderInfoLog(geom_shader_id, info_length, nullptr,
                               &shader_log[0]);
            std::stringstream msg;
            msg << "Error Compiling Geometry Shader ..." << std::endl;
            msg << shader_log;
            throw std::runtime_error(msg.str());
        }
        glAttachShader(program_id, geom_shader_id);
    }

    // Compile fragment shader
    auto fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (!shaderConfig.fs.empty()) {
        auto fragment_shader_code_ptr = shaderConfig.fs.c_str();
        glShaderSource(fragment_shader_id, 1, &fragment_shader_code_ptr, nullptr);
        glCompileShader(fragment_shader_id);

        // Check fragment shader log
        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_length);
            std::string fragment_shader_log((unsigned long) info_length, ' ');
            glGetShaderInfoLog(fragment_shader_id, info_length, nullptr,
                               &fragment_shader_log[0]);
            std::stringstream msg;
            msg << "Error Compiling Fragment Shader ..." << std::endl;
            msg << fragment_shader_log << std::endl;
            throw std::runtime_error(msg.str());
        }
        glAttachShader(program_id, fragment_shader_id);
    }

    // Create and link the program
    glBindFragDataLocation(program_id, 0, "FragmentColor");
    glLinkProgram(program_id);

    // Check program log
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_length);
        std::string program_log((unsigned long) info_length, ' ');
        glGetProgramInfoLog(program_id, info_length, nullptr, &program_log[0]);
        std::stringstream msg;
        msg << "Error Linking Shader Program ..." << std::endl;
        msg << program_log;
        throw std::runtime_error(msg.str());
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(tcs_shader_id);
    glDeleteShader(tes_shader_id);
    glDeleteShader(geom_shader_id);
    glDeleteShader(fragment_shader_id);

    program = program_id;
    use();
}

ppgso::Shader::~Shader() {
    glDeleteProgram(program);
}

void ppgso::Shader::use() const {
    glUseProgram(program);
}

GLuint ppgso::Shader::getAttribLocation(const std::string &name) const {
    use();
    return (GLuint) glGetAttribLocation(program, name.c_str());
}

GLuint ppgso::Shader::getUniformLocation(const std::string &name) const {
    use();
    return (GLuint) glGetUniformLocation(program, name.c_str());
}

void ppgso::Shader::setUniform(const std::string &name, const Texture &texture, const int id) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniform1i(uniform, id);
    texture.bind(id);
}

void ppgso::Shader::setUniform(const std::string &name, glm::mat4 matrix) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniformMatrix4fv(uniform, 1, GL_FALSE, value_ptr(matrix));
}

void ppgso::Shader::setUniform(const std::string &name, glm::mat3 matrix) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniformMatrix3fv(uniform, 1, GL_FALSE, value_ptr(matrix));
}

void ppgso::Shader::setUniform(const std::string &name, float value) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniform1f(uniform, value);
}

GLuint ppgso::Shader::getProgram() const {
    return program;
}

void ppgso::Shader::setUniform(const std::string &name, glm::vec2 vector) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniform2fv(uniform, 1, value_ptr(vector));
}

void ppgso::Shader::setUniform(const std::string &name, glm::vec3 vector) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniform3fv(uniform, 1, value_ptr(vector));
}

void ppgso::Shader::setUniform(const std::string &name, glm::vec4 vector) const {
    use();
    auto uniform = getUniformLocation(name.c_str());
    glUniform4fv(uniform, 1, value_ptr(vector));
}
