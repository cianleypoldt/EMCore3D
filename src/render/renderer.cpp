#include "render/renderer.hpp"
#include "render/math_utils.hpp"
#include <spdlog/spdlog.h>
#include <cstring>
#include <cmath>

namespace render {

// Camera implementation
Camera::Camera(float fov, float aspect_ratio, float near_plane, float far_plane)
    : fov_(fov), aspect_ratio_(aspect_ratio), near_plane_(near_plane), far_plane_(far_plane) {
    update_vectors();
    update_view_matrix();
    update_projection_matrix();
}

void Camera::set_position(const vec3& position) {
    position_ = position;
    view_dirty_ = true;
}

void Camera::set_target(const vec3& target) {
    vec3 direction = target - position_;
    forward_ = direction.normalized();
    update_vectors();
    view_dirty_ = true;
}

void Camera::set_up(const vec3& up) {
    up_ = up.normalized();
    update_vectors();
    view_dirty_ = true;
}

void Camera::move_forward(float distance) {
    position_ += forward_ * distance;
    view_dirty_ = true;
}

void Camera::move_right(float distance) {
    position_ += right_ * distance;
    view_dirty_ = true;
}

void Camera::move_up(float distance) {
    position_ += up_ * distance;
    view_dirty_ = true;
}

void Camera::rotate(float yaw_delta, float pitch_delta) {
    yaw_ += yaw_delta;
    pitch_ += pitch_delta;
    
    // Constrain pitch
    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;
    
    update_vectors();
    view_dirty_ = true;
}

void Camera::set_aspect_ratio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    projection_dirty_ = true;
}

void Camera::set_fov(float fov) {
    fov_ = fov;
    projection_dirty_ = true;
}

void Camera::update_vectors() {
    // Calculate forward vector from angles
    forward_[0] = std::cos(yaw_ * M_PI / 180.0f) * std::cos(pitch_ * M_PI / 180.0f);
    forward_[1] = std::sin(pitch_ * M_PI / 180.0f);
    forward_[2] = std::sin(yaw_ * M_PI / 180.0f) * std::cos(pitch_ * M_PI / 180.0f);
    forward_.normalize();
    
    // Calculate right and up vectors
    right_ = forward_.cross(vec3(0, 1, 0)).normalized();
    up_ = right_.cross(forward_).normalized();
}

void Camera::update_view_matrix() {
    if (!view_dirty_) return;
    
    vec3 target = position_ + forward_;
    Mat4 view = Mat4::look_at(position_[0], position_[1], position_[2],
                             target[0], target[1], target[2],
                             up_[0], up_[1], up_[2]);
    
    std::memcpy(view_matrix_, view.ptr(), sizeof(view_matrix_));
    view_dirty_ = false;
}

void Camera::update_projection_matrix() {
    if (!projection_dirty_) return;
    
    Mat4 proj = Mat4::perspective(fov_, aspect_ratio_, near_plane_, far_plane_);
    std::memcpy(projection_matrix_, proj.ptr(), sizeof(projection_matrix_));
    projection_dirty_ = false;
}

const float* Camera::get_view_matrix() const {
    const_cast<Camera*>(this)->update_view_matrix();
    return view_matrix_;
}

const float* Camera::get_projection_matrix() const {
    const_cast<Camera*>(this)->update_projection_matrix();
    return projection_matrix_;
}

// Renderer implementation
std::unique_ptr<Renderer> Renderer::create(std::unique_ptr<GLContext> context) {
    auto renderer = std::unique_ptr<Renderer>(new Renderer(std::move(context)));
    if (!renderer->initialize()) {
        return nullptr;
    }
    return renderer;
}

Renderer::Renderer(std::unique_ptr<GLContext> context) : context_(std::move(context)) {}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize() {
    if (!context_) {
        spdlog::error("Cannot initialize renderer without valid OpenGL context");
        return false;
    }
    
    // Set up viewport based on window size
    int width, height;
    context_->get_framebuffer_size(width, height);
    set_viewport(0, 0, width, height);
    camera_.set_aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
    
    // Set up framebuffer resize callback
    context_->set_framebuffer_resize_callback([](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        // Note: In a real application, you'd want to update the camera's aspect ratio here
    });
    
    initialized_ = true;
    spdlog::info("Renderer initialized successfully");
    return true;
}

void Renderer::shutdown() {
    initialized_ = false;
}

void Renderer::begin_frame() {
    if (!initialized_) return;
    context_->poll_events();
}

void Renderer::end_frame() {
    if (!initialized_) return;
    context_->swap_buffers();
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::draw_arrays(const VertexArray& vao, GLenum mode, GLint first, GLsizei count) {
    vao.bind();
    glDrawArrays(mode, first, count);
    vao.unbind();
}

void Renderer::draw_elements(const VertexArray& vao, GLenum mode, GLsizei count, GLenum type, const void* indices) {
    vao.bind();
    glDrawElements(mode, count, type, indices);
    vao.unbind();
}

void Renderer::render_field_slice(const float* field_data, int width, int height, float min_val, float max_val, const Camera& camera) {
    // TODO: Implement field slice rendering with texture mapping
    spdlog::debug("render_field_slice not yet implemented");
}

void Renderer::render_field_volume(const float* field_data, int width, int height, int depth, const Camera& camera) {
    // TODO: Implement volumetric field rendering
    spdlog::debug("render_field_volume not yet implemented");
}

bool Renderer::check_gl_error(const char* operation) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        spdlog::error("OpenGL error {} in operation: {}", error, operation ? operation : "unknown");
        return false;
    }
    return true;
}

} // namespace render