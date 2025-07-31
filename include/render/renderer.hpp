#pragma once

#include "render/gl_context.hpp"
#include "render/shader.hpp"
#include "render/buffer.hpp"
#include "common/types.hpp"

#include <memory>
#include <vector>

namespace render {

class Camera {
public:
    Camera(float fov = 45.0f, float aspect_ratio = 16.0f/9.0f, float near_plane = 0.1f, float far_plane = 1000.0f);
    
    void set_position(const vec3& position);
    void set_target(const vec3& target);
    void set_up(const vec3& up);
    
    void move_forward(float distance);
    void move_right(float distance);
    void move_up(float distance);
    
    void rotate(float yaw_delta, float pitch_delta);
    
    void set_aspect_ratio(float aspect_ratio);
    void set_fov(float fov);
    
    const float* get_view_matrix() const;
    const float* get_projection_matrix() const;
    
    vec3 get_position() const { return position_; }
    vec3 get_forward() const { return forward_; }
    vec3 get_right() const { return right_; }
    vec3 get_up() const { return up_; }

private:
    void update_view_matrix();
    void update_projection_matrix();
    void update_vectors();
    
    vec3 position_{0, 0, 5};
    vec3 forward_{0, 0, -1};
    vec3 right_{1, 0, 0};
    vec3 up_{0, 1, 0};
    
    float yaw_ = -90.0f;
    float pitch_ = 0.0f;
    
    float fov_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
    
    mutable float view_matrix_[16];
    mutable float projection_matrix_[16];
    mutable bool view_dirty_ = true;
    mutable bool projection_dirty_ = true;
};

class Renderer {
public:
    static std::unique_ptr<Renderer> create(std::unique_ptr<GLContext> context);
    
    ~Renderer();
    
    // Non-copyable
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    bool initialize();
    void shutdown();
    
    void begin_frame();
    void end_frame();
    
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    void set_viewport(int x, int y, int width, int height);
    
    // Basic rendering
    void draw_arrays(const VertexArray& vao, GLenum mode, GLint first, GLsizei count);
    void draw_elements(const VertexArray& vao, GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr);
    
    // Field visualization helpers
    void render_field_slice(const float* field_data, int width, int height, float min_val, float max_val, const Camera& camera);
    void render_field_volume(const float* field_data, int width, int height, int depth, const Camera& camera);
    
    GLContext* get_context() const { return context_.get(); }
    Camera& get_camera() { return camera_; }
    const Camera& get_camera() const { return camera_; }

private:
    explicit Renderer(std::unique_ptr<GLContext> context);
    bool check_gl_error(const char* operation = nullptr);
    
    std::unique_ptr<GLContext> context_;
    Camera camera_;
    bool initialized_ = false;
};

} // namespace render
