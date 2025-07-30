#include "render/buffer.hpp"
#include <spdlog/spdlog.h>

namespace render {

// Buffer implementation
std::unique_ptr<Buffer> Buffer::create(Type type, Usage usage) {
    return std::unique_ptr<Buffer>(new Buffer(type, usage));
}

Buffer::Buffer(Type type, Usage usage) : type_(type), usage_(usage) {
    glGenBuffers(1, &buffer_id_);
}

Buffer::~Buffer() {
    if (buffer_id_ != 0) {
        glDeleteBuffers(1, &buffer_id_);
    }
}

void Buffer::bind() const {
    glBindBuffer(static_cast<GLenum>(type_), buffer_id_);
}

void Buffer::unbind() const {
    glBindBuffer(static_cast<GLenum>(type_), 0);
}

void Buffer::set_data(const void* data, size_t size) {
    bind();
    glBufferData(static_cast<GLenum>(type_), size, data, static_cast<GLenum>(usage_));
    size_ = size;
}

void Buffer::set_sub_data(const void* data, size_t size, size_t offset) {
    bind();
    glBufferSubData(static_cast<GLenum>(type_), offset, size, data);
}

// VertexArray implementation
std::unique_ptr<VertexArray> VertexArray::create() {
    return std::unique_ptr<VertexArray>(new VertexArray());
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &vao_id_);
}

VertexArray::~VertexArray() {
    if (vao_id_ != 0) {
        glDeleteVertexArrays(1, &vao_id_);
    }
}

void VertexArray::bind() const {
    glBindVertexArray(vao_id_);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::set_vertex_buffer(std::shared_ptr<Buffer> buffer) {
    if (buffer && buffer->get_type() == Buffer::Type::Vertex) {
        vertex_buffer_ = buffer;
        bind();
        buffer->bind();
    }
}

void VertexArray::set_index_buffer(std::shared_ptr<Buffer> buffer) {
    if (buffer && buffer->get_type() == Buffer::Type::Index) {
        index_buffer_ = buffer;
        bind();
        buffer->bind();
    }
}

void VertexArray::add_vertex_attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
    bind();
    if (vertex_buffer_) {
        vertex_buffer_->bind();
    }
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

size_t VertexArray::get_index_count() const {
    if (index_buffer_) {
        // Assuming 32-bit indices (GLuint)
        return index_buffer_->get_size() / sizeof(GLuint);
    }
    return 0;
}

} // namespace render