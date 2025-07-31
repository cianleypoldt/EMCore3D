#include "render/renderer.hpp"
#include "common/spdlog_init.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

int main() {
    io::init_spdlog(spdlog::level::debug);
    
    // Test the OpenGL abstraction
    render::WindowConfig config;
    config.width = 800;
    config.height = 600;
    config.title = "OpenGL Abstraction Test";
    
    auto context = render::GLContext::create(config);
    if (!context) {
        spdlog::error("Failed to create OpenGL context");
        return -1;
    }
    
    auto renderer = render::Renderer::create(std::move(context));
    if (!renderer) {
        spdlog::error("Failed to create renderer");
        return -1;
    }
    
    spdlog::info("OpenGL abstraction initialized successfully!");
    
    // Simple render loop
    int frame_count = 0;
    while (!renderer->get_context()->should_close() && frame_count < 60) {
        renderer->begin_frame();
        
        // Clear to a blue color
        renderer->clear(0.2f, 0.3f, 0.8f, 1.0f);
        
        renderer->end_frame();
        frame_count++;
    }
    
    spdlog::info("Rendered {} frames successfully", frame_count);
    return 0;
}