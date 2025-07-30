cmake_minimum_required(VERSION 3.15)

# ------------------------------------------------------------------
#  Toolchain: Use Clang/Clang++
# ------------------------------------------------------------------
set(CMAKE_C_COMPILER   "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(emws LANGUAGES CXX)

# ------------------------------------------------------------------
#  C++ Standard and Compile Commands
# ------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------
#  Project Paths Debug
# ------------------------------------------------------------------
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# ------------------------------------------------------------------
#  Dependencies
# ------------------------------------------------------------------
find_package(OpenMP REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(spdlog REQUIRED)
find_package(GLEW REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# ------------------------------------------------------------------
#  Sources and Headers
# ------------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)

# Add main file from base project directory
set(MAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

message(STATUS "Sources found:")
foreach(SRC ${SOURCES})
    message(STATUS "  ${SRC}")
endforeach()
message(STATUS "  ${MAIN_FILE}")

# ------------------------------------------------------------------
#  Executable
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SOURCES} ${HEADERS})

# ------------------------------------------------------------------
#  Include Directories
# ------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${GLFW_INCLUDE_DIRS}
)

message(STATUS "Include directories set to:")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "  ${CMAKE_CURRENT_SOURCE_DIR}/src")

# ------------------------------------------------------------------
#  Compiler Flags
# ------------------------------------------------------------------
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:-O3 -mavx2 -mfma -march=native -flto -ffast-math -DNDEBUG>
    $<$<CONFIG:Debug>:-g -Wall -Wextra -Wpedantic>
)

# ------------------------------------------------------------------
#  Linking
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        GLEW::GLEW
        ${GLFW_LIBRARIES}
        spdlog::spdlog
        OpenMP::OpenMP_CXX
)

# ------------------------------------------------------------------
#  Compiler Info
# ------------------------------------------------------------------
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
