cmake_minimum_required(VERSION 3.15)

# ------------------------------------------------------------------
#  Toolchain: Use Clang/Clang++
# ------------------------------------------------------------------
# Set compilers BEFORE project()!
set(CMAKE_C_COMPILER   "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(emws LANGUAGES CXX)

# ------------------------------------------------------------------
#  C++ standard
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------
#  Debug: Print project paths
# ------------------------------------------------------------------
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Project name: ${PROJECT_NAME}")

# ------------------------------------------------------------------
#  Packages we need
# ------------------------------------------------------------------
find_package(OpenMP REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(spdlog REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# ------------------------------------------------------------------
#  Gather sources
# ------------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# ------------------------------------------------------------------
#  Debug: Print sources
# ------------------------------------------------------------------
message(STATUS "Sources found:")
foreach(SRC ${SOURCES})
    message(STATUS "  ${SRC}")
endforeach()

# ------------------------------------------------------------------
#  Main executable
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ------------------------------------------------------------------
#  Compiler flags (Clang)
# ------------------------------------------------------------------
# Use target_compile_options for modern CMake
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:-O3 -mavx2 -mfma -march=native -flto -ffast-math -DNDEBUG>
    $<$<CONFIG:Debug>:-g -Wall -Wextra>
)

# SIMD flags for specific file
set_source_files_properties(
    src/Simulation/Grid/yeeUpdateSIMD.cpp
    PROPERTIES COMPILE_OPTIONS "-mavx2;-mfma"
)

# ------------------------------------------------------------------
#  Include directories
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GLFW_INCLUDE_DIRS}
)

# ------------------------------------------------------------------
#  Debug: Print include directories
# ------------------------------------------------------------------
message(STATUS "Include directories set to:")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/src")

# ------------------------------------------------------------------
#  Link libraries
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glm::glm
        OpenGL::GL
        ${GLFW_LIBRARIES}
        spdlog::spdlog
        OpenMP::OpenMP_CXX
)

# ------------------------------------------------------------------
#  Print compiler info
# ------------------------------------------------------------------
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
