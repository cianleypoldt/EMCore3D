cmake_minimum_required(VERSION 3.15)
project(emws LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")

# Find required packages
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(spdlog REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Recursively gather all .cpp files in src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories (for GLFW from pkg-config)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})

# Link GLM, OpenGL, GLFW
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glm::glm
        OpenGL::GL
        ${GLFW_LIBRARIES}
        spdlog::spdlog
)
