#pragma once

#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace render {

class Shader {
public:
    enum class Type {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Compute = GL_COMPUTE_SHADER
    };

    static std::unique_ptr<Shader> create_from_source(const std::string& vertex_source, 
                                                     const std::string& fragment_source);
    static std::unique_ptr<Shader> create_from_files(const std::string& vertex_path,
                                                     const std::string& fragment_path);
    
    ~Shader();
    
    // Non-copyable
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    void bind() const;
    void unbind() const;
    
    // Uniform setters
    void set_uniform(const std::string& name, int value);
    void set_uniform(const std::string& name, float value);
    void set_uniform(const std::string& name, const float* matrix4, bool transpose = false);
    void set_uniform(const std::string& name, float x, float y);
    void set_uniform(const std::string& name, float x, float y, float z);
    void set_uniform(const std::string& name, float x, float y, float z, float w);
    
    GLuint get_program_id() const { return program_id_; }
    bool is_valid() const { return program_id_ != 0; }

private:
    Shader() = default;
    bool compile_and_link(const std::string& vertex_source, const std::string& fragment_source);
    bool compile_shader(GLuint shader_id, const std::string& source);
    GLint get_uniform_location(const std::string& name);
    
    GLuint program_id_ = 0;
    mutable std::unordered_map<std::string, GLint> uniform_locations_;
};

} // namespace render