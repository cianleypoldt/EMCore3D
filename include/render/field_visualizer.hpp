#pragma once

#include "render/renderer.hpp"
#include "render/shader.hpp"
#include "render/buffer.hpp"
#include "yee/grid.hpp"
#include <memory>
#include <vector>

namespace render {

class FieldVisualizer {
public:
    static std::unique_ptr<FieldVisualizer> create(Renderer* renderer);
    
    ~FieldVisualizer() = default;
    
    // Non-copyable
    FieldVisualizer(const FieldVisualizer&) = delete;
    FieldVisualizer& operator=(const FieldVisualizer&) = delete;
    
    bool initialize();
    
    // Render a 2D slice of the electromagnetic field
    void render_ex_slice(const YeeGrid& grid, int z_slice, const Camera& camera);
    void render_ey_slice(const YeeGrid& grid, int z_slice, const Camera& camera);
    void render_ez_slice(const YeeGrid& grid, int z_slice, const Camera& camera);
    
    // Render field magnitude
    void render_e_magnitude_slice(const YeeGrid& grid, int z_slice, const Camera& camera);
    void render_h_magnitude_slice(const YeeGrid& grid, int z_slice, const Camera& camera);
    
    // Set visualization parameters
    void set_color_scale(float min_val, float max_val);
    void set_slice_position(float z_offset);

private:
    explicit FieldVisualizer(Renderer* renderer);
    
    void update_mesh_data(const YeeGrid& grid, int z_slice, const std::function<float(const YeeGrid&, const index3&)>& field_getter);
    void find_field_range(const YeeGrid& grid, int z_slice, const std::function<float(const YeeGrid&, const index3&)>& field_getter, float& min_val, float& max_val);
    
    struct Vertex {
        float x, y, z;
        float field_value;
    };
    
    Renderer* renderer_;
    std::unique_ptr<Shader> field_shader_;
    std::unique_ptr<VertexArray> vao_;
    std::shared_ptr<Buffer> vertex_buffer_;
    std::shared_ptr<Buffer> index_buffer_;
    
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    
    float min_value_ = -1.0f;
    float max_value_ = 1.0f;
    float z_offset_ = 0.0f;
    
    bool initialized_ = false;
};

} // namespace render