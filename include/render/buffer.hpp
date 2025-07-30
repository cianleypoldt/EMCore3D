#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>

namespace render {

class Buffer {
public:
    enum class Type {
        Vertex = GL_ARRAY_BUFFER,
        Index = GL_ELEMENT_ARRAY_BUFFER,
        Uniform = GL_UNIFORM_BUFFER
    };
    
    enum class Usage {
        Static = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream = GL_STREAM_DRAW
    };

    static std::unique_ptr<Buffer> create(Type type, Usage usage = Usage::Static);
    
    ~Buffer();
    
    // Non-copyable
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    
    void bind() const;
    void unbind() const;
    
    void set_data(const void* data, size_t size);
    void set_sub_data(const void* data, size_t size, size_t offset = 0);
    
    template<typename T>
    void set_data(const std::vector<T>& data) {
        set_data(data.data(), data.size() * sizeof(T));
    }
    
    GLuint get_id() const { return buffer_id_; }
    Type get_type() const { return type_; }
    size_t get_size() const { return size_; }

private:
    explicit Buffer(Type type, Usage usage);
    
    GLuint buffer_id_ = 0;
    Type type_;
    Usage usage_;
    size_t size_ = 0;
};

class VertexArray {
public:
    static std::unique_ptr<VertexArray> create();
    
    ~VertexArray();
    
    // Non-copyable
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    
    void bind() const;
    void unbind() const;
    
    void set_vertex_buffer(std::shared_ptr<Buffer> buffer);
    void set_index_buffer(std::shared_ptr<Buffer> buffer);
    
    void add_vertex_attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
    
    GLuint get_id() const { return vao_id_; }
    size_t get_index_count() const;

private:
    VertexArray();
    
    GLuint vao_id_ = 0;
    std::shared_ptr<Buffer> vertex_buffer_;
    std::shared_ptr<Buffer> index_buffer_;
};

} // namespace render