#include "render/renderer.hpp"
#include "render/field_visualizer.hpp"
#include "app/simulation.hpp"
#include "common/spdlog_init.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

int main() {
    io::init_spdlog(spdlog::level::debug);
    
    spdlog::info("Starting EMCore3D Field Visualization Example");
    
    // Create simulation
    Simulation sim({50, 50, 20});
    
    // Add some field excitation for visualization
    for (int i = 20; i < 30; ++i) {
        for (int j = 20; j < 30; ++j) {
            for (int k = 5; k < 15; ++k) {
                index3 idx(i, j, k);
                sim.grid.ex(idx) = std::sin(i * 0.2) * std::cos(j * 0.2);
                sim.grid.ey(idx) = std::cos(i * 0.2) * std::sin(j * 0.2);
                sim.grid.ez(idx) = std::sin(i * 0.1) * std::sin(j * 0.1);
            }
        }
    }
    
    // Try to create OpenGL context (will fail in headless environment, but demonstrates the API)
    render::WindowConfig config;
    config.width = 1024;
    config.height = 768;
    config.title = "EMCore3D Field Visualization";
    
    auto context = render::GLContext::create(config);
    if (!context) {
        spdlog::error("Failed to create OpenGL context (expected in headless environment)");
        spdlog::info("OpenGL abstraction layer is ready for use in windowed environments");
        return 0;
    }
    
    auto renderer = render::Renderer::create(std::move(context));
    if (!renderer) {
        spdlog::error("Failed to create renderer");
        return -1;
    }
    
    auto field_viz = render::FieldVisualizer::create(renderer.get());
    if (!field_viz) {
        spdlog::error("Failed to create field visualizer");
        return -1;
    }
    
    spdlog::info("Visualization system initialized successfully!");
    
    // Configure camera
    auto& camera = renderer->get_camera();
    camera.set_position(vec3(25, 25, 50));
    camera.set_target(vec3(25, 25, 10));
    
    // Render loop
    int frame_count = 0;
    while (!renderer->get_context()->should_close() && frame_count < 300) {
        renderer->begin_frame();
        renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
        
        // Visualize E field magnitude at z=10
        field_viz->render_e_magnitude_slice(sim.grid, 10, camera);
        
        renderer->end_frame();
        frame_count++;
        
        // Update simulation occasionally
        if (frame_count % 30 == 0) {
            sim.update(1);
            spdlog::info("Simulation step {}, frame {}", frame_count / 30, frame_count);
        }
    }
    
    spdlog::info("Visualization completed successfully with {} frames", frame_count);
    return 0;
}