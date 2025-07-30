#include "render/shader.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

namespace render {

std::unique_ptr<Shader> Shader::create_from_source(const std::string& vertex_source, 
                                                   const std::string& fragment_source) {
    auto shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->compile_and_link(vertex_source, fragment_source)) {
        return nullptr;
    }
    return shader;
}

std::unique_ptr<Shader> Shader::create_from_files(const std::string& vertex_path,
                                                  const std::string& fragment_path) {
    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);
    
    if (!vertex_file.is_open()) {
        spdlog::error("Could not open vertex shader file: {}", vertex_path);
        return nullptr;
    }
    
    if (!fragment_file.is_open()) {
        spdlog::error("Could not open fragment shader file: {}", fragment_path);
        return nullptr;
    }
    
    std::stringstream vertex_stream, fragment_stream;
    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();
    
    return create_from_source(vertex_stream.str(), fragment_stream.str());
}

Shader::~Shader() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
}

void Shader::bind() const {
    if (program_id_ != 0) {
        glUseProgram(program_id_);
    }
}

void Shader::unbind() const {
    glUseProgram(0);
}

bool Shader::compile_and_link(const std::string& vertex_source, const std::string& fragment_source) {
    // Create vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (!compile_shader(vertex_shader, vertex_source)) {
        glDeleteShader(vertex_shader);
        return false;
    }
    
    // Create fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (!compile_shader(fragment_shader, fragment_source)) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return false;
    }
    
    // Create program
    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_shader);
    glAttachShader(program_id_, fragment_shader);
    glLinkProgram(program_id_);
    
    // Check linking
    GLint success;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(program_id_, 512, nullptr, info_log);
        spdlog::error("Shader program linking failed: {}", info_log);
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(program_id_);
        program_id_ = 0;
        return false;
    }
    
    // Clean up shaders (they're linked into the program now)
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    spdlog::debug("Shader program created successfully (ID: {})", program_id_);
    return true;
}

bool Shader::compile_shader(GLuint shader_id, const std::string& source) {
    const char* src = source.c_str();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);
    
    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
        spdlog::error("Shader compilation failed: {}", info_log);
        return false;
    }
    
    return true;
}

GLint Shader::get_uniform_location(const std::string& name) {
    auto it = uniform_locations_.find(name);
    if (it != uniform_locations_.end()) {
        return it->second;
    }
    
    GLint location = glGetUniformLocation(program_id_, name.c_str());
    uniform_locations_[name] = location;
    
    if (location == -1) {
        spdlog::warn("Uniform '{}' not found in shader program {}", name, program_id_);
    }
    
    return location;
}

void Shader::set_uniform(const std::string& name, int value) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::set_uniform(const std::string& name, float value) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::set_uniform(const std::string& name, const float* matrix4, bool transpose) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, matrix4);
    }
}

void Shader::set_uniform(const std::string& name, float x, float y) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniform2f(location, x, y);
    }
}

void Shader::set_uniform(const std::string& name, float x, float y, float z) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

void Shader::set_uniform(const std::string& name, float x, float y, float z, float w) {
    GLint location = get_uniform_location(name);
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

} // namespace render