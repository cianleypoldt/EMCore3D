#include "render/field_visualizer.hpp"
#include "render/math_utils.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>

namespace render {

std::unique_ptr<FieldVisualizer> FieldVisualizer::create(Renderer* renderer) {
    auto visualizer = std::unique_ptr<FieldVisualizer>(new FieldVisualizer(renderer));
    if (!visualizer->initialize()) {
        return nullptr;
    }
    return visualizer;
}

FieldVisualizer::FieldVisualizer(Renderer* renderer) : renderer_(renderer) {}

bool FieldVisualizer::initialize() {
    if (!renderer_) {
        spdlog::error("FieldVisualizer: Invalid renderer");
        return false;
    }
    
    // Load shaders from files
    std::ifstream vertex_file("shaders/field_vertex.glsl");
    std::ifstream fragment_file("shaders/field_fragment.glsl");
    
    if (!vertex_file.is_open() || !fragment_file.is_open()) {
        spdlog::error("FieldVisualizer: Could not open shader files");
        return false;
    }
    
    std::stringstream vertex_stream, fragment_stream;
    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();
    
    field_shader_ = Shader::create_from_source(vertex_stream.str(), fragment_stream.str());
    if (!field_shader_) {
        spdlog::error("FieldVisualizer: Failed to create field shader");
        return false;
    }
    
    // Create vertex array and buffers
    vao_ = VertexArray::create();
    vertex_buffer_ = Buffer::create(Buffer::Type::Vertex, Buffer::Usage::Dynamic);
    index_buffer_ = Buffer::create(Buffer::Type::Index, Buffer::Usage::Dynamic);
    
    if (!vao_ || !vertex_buffer_ || !index_buffer_) {
        spdlog::error("FieldVisualizer: Failed to create OpenGL objects");
        return false;
    }
    
    vao_->set_vertex_buffer(vertex_buffer_);
    vao_->set_index_buffer(index_buffer_);
    
    // Set up vertex attributes
    vao_->add_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    vao_->add_vertex_attribute(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, field_value));
    
    initialized_ = true;
    spdlog::info("FieldVisualizer initialized successfully");
    return true;
}

void FieldVisualizer::render_ex_slice(const YeeGrid& grid, int z_slice, const Camera& camera) {
    auto ex_getter = [](const YeeGrid& g, const index3& idx) -> float {
        return g.ex(idx);
    };
    update_mesh_data(grid, z_slice, ex_getter);
    
    // Auto-scale color range
    float min_val, max_val;
    find_field_range(grid, z_slice, ex_getter, min_val, max_val);
    set_color_scale(min_val, max_val);
    
    // Render
    field_shader_->bind();
    field_shader_->set_uniform("uView", camera.get_view_matrix());
    field_shader_->set_uniform("uProjection", camera.get_projection_matrix());
    
    Mat4 model;
    field_shader_->set_uniform("uModel", model.ptr());
    field_shader_->set_uniform("uMinValue", min_value_);
    field_shader_->set_uniform("uMaxValue", max_value_);
    
    if (!indices_.empty()) {
        renderer_->draw_elements(*vao_, GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT);
    }
    
    field_shader_->unbind();
}

void FieldVisualizer::render_ey_slice(const YeeGrid& grid, int z_slice, const Camera& camera) {
    auto ey_getter = [](const YeeGrid& g, const index3& idx) -> float {
        return g.ey(idx);
    };
    update_mesh_data(grid, z_slice, ey_getter);
    
    float min_val, max_val;
    find_field_range(grid, z_slice, ey_getter, min_val, max_val);
    set_color_scale(min_val, max_val);
    
    field_shader_->bind();
    field_shader_->set_uniform("uView", camera.get_view_matrix());
    field_shader_->set_uniform("uProjection", camera.get_projection_matrix());
    
    Mat4 model;
    field_shader_->set_uniform("uModel", model.ptr());
    field_shader_->set_uniform("uMinValue", min_value_);
    field_shader_->set_uniform("uMaxValue", max_value_);
    
    if (!indices_.empty()) {
        renderer_->draw_elements(*vao_, GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT);
    }
    
    field_shader_->unbind();
}

void FieldVisualizer::render_ez_slice(const YeeGrid& grid, int z_slice, const Camera& camera) {
    auto ez_getter = [](const YeeGrid& g, const index3& idx) -> float {
        return g.ez(idx);
    };
    update_mesh_data(grid, z_slice, ez_getter);
    
    float min_val, max_val;
    find_field_range(grid, z_slice, ez_getter, min_val, max_val);
    set_color_scale(min_val, max_val);
    
    field_shader_->bind();
    field_shader_->set_uniform("uView", camera.get_view_matrix());
    field_shader_->set_uniform("uProjection", camera.get_projection_matrix());
    
    Mat4 model;
    field_shader_->set_uniform("uModel", model.ptr());
    field_shader_->set_uniform("uMinValue", min_value_);
    field_shader_->set_uniform("uMaxValue", max_value_);
    
    if (!indices_.empty()) {
        renderer_->draw_elements(*vao_, GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT);
    }
    
    field_shader_->unbind();
}

void FieldVisualizer::render_e_magnitude_slice(const YeeGrid& grid, int z_slice, const Camera& camera) {
    auto e_mag_getter = [](const YeeGrid& g, const index3& idx) -> float {
        float ex = g.ex(idx);
        float ey = g.ey(idx);
        float ez = g.ez(idx);
        return std::sqrt(ex*ex + ey*ey + ez*ez);
    };
    update_mesh_data(grid, z_slice, e_mag_getter);
    
    float min_val, max_val;
    find_field_range(grid, z_slice, e_mag_getter, min_val, max_val);
    set_color_scale(min_val, max_val);
    
    field_shader_->bind();
    field_shader_->set_uniform("uView", camera.get_view_matrix());
    field_shader_->set_uniform("uProjection", camera.get_projection_matrix());
    
    Mat4 model;
    field_shader_->set_uniform("uModel", model.ptr());
    field_shader_->set_uniform("uMinValue", min_value_);
    field_shader_->set_uniform("uMaxValue", max_value_);
    
    if (!indices_.empty()) {
        renderer_->draw_elements(*vao_, GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT);
    }
    
    field_shader_->unbind();
}

void FieldVisualizer::render_h_magnitude_slice(const YeeGrid& grid, int z_slice, const Camera& camera) {
    auto h_mag_getter = [](const YeeGrid& g, const index3& idx) -> float {
        float hx = g.hx(idx);
        float hy = g.hy(idx);
        float hz = g.hz(idx);
        return std::sqrt(hx*hx + hy*hy + hz*hz);
    };
    update_mesh_data(grid, z_slice, h_mag_getter);
    
    float min_val, max_val;
    find_field_range(grid, z_slice, h_mag_getter, min_val, max_val);
    set_color_scale(min_val, max_val);
    
    field_shader_->bind();
    field_shader_->set_uniform("uView", camera.get_view_matrix());
    field_shader_->set_uniform("uProjection", camera.get_projection_matrix());
    
    Mat4 model;
    field_shader_->set_uniform("uModel", model.ptr());
    field_shader_->set_uniform("uMinValue", min_value_);
    field_shader_->set_uniform("uMaxValue", max_value_);
    
    if (!indices_.empty()) {
        renderer_->draw_elements(*vao_, GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT);
    }
    
    field_shader_->unbind();
}

void FieldVisualizer::set_color_scale(float min_val, float max_val) {
    min_value_ = min_val;
    max_value_ = max_val;
}

void FieldVisualizer::set_slice_position(float z_offset) {
    z_offset_ = z_offset;
}

void FieldVisualizer::update_mesh_data(const YeeGrid& grid, int z_slice, const std::function<float(const YeeGrid&, const index3&)>& field_getter) {
    if (z_slice < 0 || z_slice >= grid.grid_dimensions[2]) {
        spdlog::warn("FieldVisualizer: z_slice {} out of bounds [0, {})", z_slice, grid.grid_dimensions[2]);
        return;
    }
    
    vertices_.clear();
    indices_.clear();
    
    int width = grid.grid_dimensions[0];
    int height = grid.grid_dimensions[1];
    
    // Generate vertices
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            index3 idx(i, j, z_slice);
            float field_value = field_getter(grid, idx);
            
            // Map grid coordinates to world coordinates
            float x = (float)i * grid.cell_dimensions[0];
            float y = (float)j * grid.cell_dimensions[1];
            float z = (float)z_slice * grid.cell_dimensions[2] + z_offset_;
            
            vertices_.push_back({x, y, z, field_value});
        }
    }
    
    // Generate indices for triangles
    for (int j = 0; j < height - 1; ++j) {
        for (int i = 0; i < width - 1; ++i) {
            unsigned int v0 = j * width + i;
            unsigned int v1 = j * width + (i + 1);
            unsigned int v2 = (j + 1) * width + i;
            unsigned int v3 = (j + 1) * width + (i + 1);
            
            // First triangle
            indices_.push_back(v0);
            indices_.push_back(v1);
            indices_.push_back(v2);
            
            // Second triangle
            indices_.push_back(v1);
            indices_.push_back(v3);
            indices_.push_back(v2);
        }
    }
    
    // Upload to GPU
    vertex_buffer_->set_data(vertices_);
    index_buffer_->set_data(indices_);
}

void FieldVisualizer::find_field_range(const YeeGrid& grid, int z_slice, const std::function<float(const YeeGrid&, const index3&)>& field_getter, float& min_val, float& max_val) {
    min_val = std::numeric_limits<float>::max();
    max_val = std::numeric_limits<float>::lowest();
    
    int width = grid.grid_dimensions[0];
    int height = grid.grid_dimensions[1];
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            index3 idx(i, j, z_slice);
            float value = field_getter(grid, idx);
            min_val = std::min(min_val, value);
            max_val = std::max(max_val, value);
        }
    }
    
    // Avoid degenerate case
    if (std::abs(max_val - min_val) < 1e-6f) {
        max_val = min_val + 1.0f;
    }
}

} // namespace render