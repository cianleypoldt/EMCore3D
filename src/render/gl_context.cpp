#include "render/gl_context.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace render {

bool GLContext::glfw_initialized_ = false;

std::unique_ptr<GLContext> GLContext::create(const WindowConfig& config) {
    auto context = std::unique_ptr<GLContext>(new GLContext(config));
    if (!context->initialize()) {
        return nullptr;
    }
    return context;
}

GLContext::GLContext(const WindowConfig& config) : config_(config) {}

GLContext::~GLContext() {
    cleanup();
}

bool GLContext::initialize() {
    if (!glfw_initialized_) {
        if (!glfwInit()) {
            spdlog::error("Failed to initialize GLFW");
            return false;
        }
        glfw_initialized_ = true;
        
        glfwSetErrorCallback([](int error, const char* description) {
            spdlog::error("GLFW Error {}: {}", error, description);
        });
    }
    
    // Set OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config_.resizable ? GLFW_TRUE : GLFW_FALSE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    window_ = glfwCreateWindow(config_.width, config_.height, config_.title.c_str(), nullptr, nullptr);
    if (!window_) {
        spdlog::error("Failed to create GLFW window");
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    
    // Initialize GLEW after creating context
    if (glewInit() != GLEW_OK) {
        spdlog::error("Failed to initialize GLEW");
        return false;
    }
    
    if (config_.vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
    
    // Set default viewport
    glViewport(0, 0, config_.width, config_.height);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    spdlog::info("OpenGL Context created: {}x{}", config_.width, config_.height);
    spdlog::info("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    spdlog::info("OpenGL Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    
    return true;
}

void GLContext::cleanup() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}

bool GLContext::should_close() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

void GLContext::swap_buffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

void GLContext::poll_events() {
    glfwPollEvents();
}

void GLContext::get_framebuffer_size(int& width, int& height) const {
    if (window_) {
        glfwGetFramebufferSize(window_, &width, &height);
    } else {
        width = height = 0;
    }
}

void GLContext::set_framebuffer_resize_callback(void(*callback)(GLFWwindow*, int, int)) {
    if (window_) {
        glfwSetFramebufferSizeCallback(window_, callback);
    }
}

void GLContext::set_key_callback(void(*callback)(GLFWwindow*, int, int, int, int)) {
    if (window_) {
        glfwSetKeyCallback(window_, callback);
    }
}

void GLContext::set_mouse_button_callback(void(*callback)(GLFWwindow*, int, int, int)) {
    if (window_) {
        glfwSetMouseButtonCallback(window_, callback);
    }
}

void GLContext::set_cursor_pos_callback(void(*callback)(GLFWwindow*, double, double)) {
    if (window_) {
        glfwSetCursorPosCallback(window_, callback);
    }
}

void GLContext::set_scroll_callback(void(*callback)(GLFWwindow*, double, double)) {
    if (window_) {
        glfwSetScrollCallback(window_, callback);
    }
}

} // namespace render