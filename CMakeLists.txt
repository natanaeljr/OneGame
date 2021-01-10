cmake_minimum_required(VERSION 3.5)
project(FirstGame LANGUAGES CXX)

# This project contains only the game logic build part. Therefore it is intended to be
# built along with the platform specific code which implements the interface with the
# underlying operating system.

# The project can be built as a standalone project on a system with the required
# libraries already installed, or it can be part of the Super-Project which builds all
# dependencies with the correct versions.
# For the latter, it is recommend that this directory is included as a sub-directory
# of the Super-project and the options set accordingly before that.
# For building it separately with the dependency builds from the Super-Project,
# the variable `CMAKE_PREFIX_PATH` must be set to the Super-Project's build/external dir.
# In Linux you can do it as:
#  cmake .. -DCMAKE_PREFIX_PATH=$(realpath ../../build/external)

#########################################################################################
# Options
#########################################################################################
option(FIRSTGAME_OPENGL_ES3        "OpenGL ES 3.0"            OFF)
option(FIRSTGAME_OPENGL_GLAD       "OpenGL Loader GLAD"       OFF)
option(FIRSTGAME_OPENGL_GLBINDING3 "OpenGL API C++ glbinding" OFF)

#########################################################################################
# Configuration
#########################################################################################
# Default build type to Debug, for release use `cmake -DCMAKE_BUILD_TYPE=Release ..`
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type default to Debug" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Turn on when having trouble building
set(CMAKE_VERBOSE_MAKEFILE OFF)
# Compilation Database for C++ language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# For OpenGL prefer new GL Vendor Neutral Dispatch (GLVND) rather than legacy
set(OpenGL_GL_PREFERENCE "GLVND")

# Default compilation settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -pg -fsanitize=undefined -Werror -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

#########################################################################################
# Dependencies
#########################################################################################
# GLAD for opengl function loader
if(${FIRSTGAME_OPENGL_GLAD})
find_package(glad 0.1.34 EXACT REQUIRED)
endif()
# GLBinding for C++ binding of the OpenGL API
if(${FIRSTGAME_OPENGL_GLBINDING3})
find_package(glbinding REQUIRED)
endif()
# OpenGL for graphics library
find_package(OpenGL REQUIRED)
# SPDLog for fast C++ logging library
find_package(spdlog 1.8.1 EXACT REQUIRED)
# GSL for types and functions of C++ Core Guidelines
find_package(Microsoft.GSL 3.1.0 EXACT REQUIRED)
# ImGui for graphical user interface
find_package(imgui 1.79 EXACT REQUIRED)
# GLM for opengl mathematics library
find_package(glm 0.9.9.9 EXACT REQUIRED)
# STB for image loading/decoding
find_package(stb 0.8.1 EXACT REQUIRED COMPONENTS stb_image)
# Assimp for 3D model loading
find_package(assimp 5.0.0 EXACT REQUIRED)
# EnTT for Entity Component System library
find_package(EnTT 3.5.2 EXACT REQUIRED)

# IrrXML for Assimp dependency
find_library(IrrXML_LIBRARY NAMES IrrXML IrrXMLd)
# Zlib for Assimp dependency
find_library(Zlib_LIBRARY NAMES zlibstatic zlibstaticd)

#########################################################################################
# Project
#########################################################################################
# Main target binary
add_library(FirstGame STATIC)
target_sources(FirstGame PRIVATE
    src/firstgame/firstgame.cpp
    src/firstgame/render/renderer.cpp
    src/firstgame/render/painter.cpp
    src/firstgame/render/camera_system.cpp
    src/firstgame/system/asset_mgr.cpp
)
target_link_libraries(FirstGame PUBLIC
    Microsoft.GSL::GSL
    spdlog::spdlog
    imgui::imgui
    glm::glm_static
    stb::stb_image
    assimp::assimp
    ${IrrXML_LIBRARY}
    ${Zlib_LIBRARY}
    EnTT::EnTT
    $<$<BOOL:${FIRSTGAME_OPENGL_GLAD}>:glad::glad>
    $<$<BOOL:${FIRSTGAME_OPENGL_GLBINDING3}>:glbinding::glbinding>
    $<$<BOOL:${FIRSTGAME_OPENGL_GLBINDING3}>:glbinding::glbinding-aux>
    OpenGL::GL
)
target_include_directories(FirstGame PRIVATE src PUBLIC include)
target_compile_definitions(FirstGame PRIVATE
    GLFW_INCLUDE_NONE
    $<$<BOOL:${FIRSTGAME_OPENGL_ES3}>:FIRSTGAME_OPENGL_ES3>
    $<$<BOOL:${FIRSTGAME_OPENGL_GLAD}>:FIRSTGAME_OPENGL_GLAD>
    $<$<BOOL:${FIRSTGAME_OPENGL_GLBINDING3}>:FIRSTGAME_OPENGL_GLBINDING3>
    FIRSTGAME_ASSETS_DIR_PATH=$<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>,"${CMAKE_CURRENT_SOURCE_DIR}/assets","TODO">
    SPDLOG_ACTIVE_LEVEL=$<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>,SPDLOG_LEVEL_TRACE,SPDLOG_LEVEL_INFO>
)

install(TARGETS FirstGame)
