# EMCore3D OpenGL Abstraction

## Overview

This directory contains a modern OpenGL abstraction layer designed for visualizing electromagnetic field simulations. The abstraction provides a clean, type-safe interface for rendering electromagnetic field data from Yee grids.

## Components

### Core Rendering Classes

- **GLContext**: Manages OpenGL context and GLFW window creation with proper error handling
- **Shader**: Handles shader compilation, linking, and uniform management
- **Buffer**: Type-safe wrapper for OpenGL buffers (VBO, VAO, EBO) with RAII semantics
- **Renderer**: High-level rendering interface with built-in camera management
- **Camera**: 3D camera with view/projection matrix management and common operations

### Field Visualization

- **FieldVisualizer**: Specialized renderer for electromagnetic field data
  - Supports Ex, Ey, Ez field component visualization
  - Electric and magnetic field magnitude rendering
  - Color-mapped field visualization with automatic scaling
  - 2D slice rendering from 3D field data

## Usage Example

```cpp
#include "render/renderer.hpp"
#include "render/field_visualizer.hpp"
#include "app/simulation.hpp"

// Create OpenGL context
render::WindowConfig config;
config.width = 1024;
config.height = 768;
auto context = render::GLContext::create(config);
auto renderer = render::Renderer::create(std::move(context));

// Create field visualizer
auto field_viz = render::FieldVisualizer::create(renderer.get());

// Set up camera
auto& camera = renderer->get_camera();
camera.set_position(vec3(25, 25, 50));
camera.set_target(vec3(25, 25, 10));

// Render electromagnetic field
Simulation sim({100, 100, 50});
field_viz->render_e_magnitude_slice(sim.grid, 25, camera);
```

## Shaders

The system includes specialized shaders for field visualization:

- `field_vertex.glsl`: Vertex shader with field value pass-through
- `field_fragment.glsl`: Fragment shader with blue-to-red field value color mapping

## Dependencies

- OpenGL 3.3+
- GLFW3 (window management)
- GLEW (OpenGL extension loading)
- spdlog (logging)
- Eigen3 (vector math integration)

## Build Integration

The OpenGL abstraction is automatically built with the main project through CMake's GLOB_RECURSE source discovery. All rendering components are in the `render` namespace.