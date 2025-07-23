cmake_minimum_required(VERSION 3.15)
project(emws LANGUAGES CXX)

# ------------------------------------------------------------------
#  Toolchain & C++ standard
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
#  Main executable
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ------------------------------------------------------------------
#  Compiler flags
# ------------------------------------------------------------------
#   Release: AVX2 + FMA + OpenMP + LTO + fast-math
set(CMAKE_CXX_FLAGS_RELEASE
    "-O3 -mavx2 -mfma -march=native -flto -ffast-math -DNDEBUG")

#   Debug (optional, but useful)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

# after add_executable(...)
set_source_files_properties(
    src/Simulation/Grid/yeeUpdateSIMD.cpp
    PROPERTIES COMPILE_OPTIONS "-mavx2;-mfma"
)

# ------------------------------------------------------------------
#  Include / link
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glm::glm
        OpenGL::GL
        ${GLFW_LIBRARIES}
        spdlog::spdlog
        OpenMP::OpenMP_CXX
)
