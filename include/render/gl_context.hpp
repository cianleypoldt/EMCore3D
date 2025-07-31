#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace render {

struct WindowConfig {
    int width = 1280;
    int height = 720;
    std::string title = "EMCore3D";
    bool vsync = true;
    bool resizable = true;
};

class GLContext {
public:

    static std::unique_ptr<GLContext> create(const WindowConfig& config = {});
    
    ~GLContext();
    
    // Non-copyable
    GLContext(const GLContext&) = delete;
    GLContext& operator=(const GLContext&) = delete;
    
    bool should_close() const;
    void swap_buffers();
    void poll_events();
    
    void get_framebuffer_size(int& width, int& height) const;
    GLFWwindow* get_window() const { return window_; }
    
    // Set callbacks
    void set_framebuffer_resize_callback(void(*callback)(GLFWwindow*, int, int));
    void set_key_callback(void(*callback)(GLFWwindow*, int, int, int, int));
    void set_mouse_button_callback(void(*callback)(GLFWwindow*, int, int, int));
    void set_cursor_pos_callback(void(*callback)(GLFWwindow*, double, double));
    void set_scroll_callback(void(*callback)(GLFWwindow*, double, double));

private:
    explicit GLContext(const WindowConfig& config);
    bool initialize();
    void cleanup();
    
    GLFWwindow* window_ = nullptr;
    WindowConfig config_;
    static bool glfw_initialized_;
};

} // namespace render