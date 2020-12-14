# This CMake script is designed for CLion IDE and QTCreator projects on Windows
# It should find bundled binary dependencies automatically when using MinGW compiler
# On Linux and Mac the dependencies (glm, glew, glfw) need to be installed manually

cmake_minimum_required(VERSION 3.1)
project(ppgso CXX)

#
# CONFIGURATION
#

# Basic CMake settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GLEW_USE_STATIC_LIBS TRUE)

# Use add_resources function to convert .glsl to a .cpp files which will be compiled into a separate static library
include(add_resources)

#
# DEPENDENCIES
#

# Set up external dependencies for Windows users
if (MINGW)
  set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/mingw")
elseif (MSVC)
  set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/vc2015")
  set(USE_STRICT_COMPILE_WARNINGS OFF CACHE BOOL "" FORCE)
endif ()

# Warnings for Debug mode
option(USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON)
# These compile flags should apply for GCC and Clang compilers
set(STRICT_COMPILE_FLAGS "-Wpedantic -Wall -Wno-c++98-compat -Wextra -Wconversion -Wsign-compare -Wuninitialized -Wno-unused-parameter")
if (USE_STRICT_COMPILE_WARNINGS)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STRICT_COMPILE_FLAGS}")
endif ()

# Find required packages
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)

# Optional packages
find_package(OpenMP)
if(OPENMP_FOUND)
  list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
endif()

# Set default installation destination
if (NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "../_install")
endif ()

#
# Shaders / Materials
#

set(PPGSO_SHADER_SRC
        shader/color_vert.glsl shader/color_frag.glsl
        shader/convolution_vert.glsl shader/convolution_frag.glsl
        shader/diffuse_vert.glsl shader/diffuse_frag.glsl
        shader/texture_vert.glsl shader/texture_frag.glsl
        shader/water_vert.glsl shader/water_tcs.glsl shader/water_tes.glsl shader/water_gs.glsl
        shader/foresail_diffuse_vert.glsl
        shader/text_vert.glsl shader/text_frag.glsl
        shader/bird_vert.glsl
        )
add_resources(shaders ${PPGSO_SHADER_SRC})

# PPGSO library
add_library(ppgso STATIC
        ppgso/mesh.cpp
        ppgso/tiny_obj_loader.cpp
        ppgso/shader.cpp
        ppgso/image.cpp
        ppgso/image_bmp.cpp
        ppgso/image_raw.cpp
        ppgso/texture.cpp
        ppgso/window.cpp
        )

# Make sure GLM uses radians and GLEW is a static library
target_compile_definitions(ppgso PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

# Link to GLFW, GLEW and OpenGL
target_link_libraries(ppgso PUBLIC ${GLFW_LIBRARIES} GLEW::GLEW ${OPENGL_LIBRARIES})
# Pass on include directories
target_include_directories(ppgso PUBLIC
        ppgso
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

#
# TARGETS
#

# Playground target
add_executable(main src/main.cpp src/scene/object.cpp src/scene/scene.cpp src/scene/scene.h src/objects/boat/boat.cpp src/scene/camera.cpp src/objects/world/water.cpp src/objects/world/water.h src/objects/world/island.cpp src/objects/world/island.h src/scene/vertex_object.cpp src/scene/vertex_object.h src/util/bezier.cpp src/util/bezier.h src/objects/world/chunk.cpp src/objects/world/chunk.h src/objects/world/tree.cpp src/objects/world/tree.h src/objects/boat/boat_wheel.cpp src/objects/boat/boat_wheel.h src/objects/boat/foresail.cpp src/objects/boat/foresail.h src/objects/boat/mainsail.cpp src/objects/boat/mainsail.h src/objects/boat/wind_vane.cpp src/objects/boat/wind_vane.h src/gui/compass.cpp src/gui/compass.h src/gui/compass_hand.cpp src/gui/compass_hand.h src/objects/sky/skydome.cpp src/objects/sky/skydome.h src/gui/text.cpp src/gui/text.h src/scene/scenes/menu_scene.cpp src/scene/scenes/menu_scene.h src/scene/scenes/game_scene.cpp src/scene/scenes/game_scene.h src/objects/world/lighthouse.cpp src/objects/world/lighthouse.h src/util/animator.cpp src/util/animator.h src/scene/executable.h src/gui/gui_boat.cpp src/gui/gui_boat.h src/objects/world/tree_bird.cpp src/objects/world/tree_bird.h src/gui/screen_overlay.cpp src/gui/screen_overlay.h src/scene_window.cpp src/scene_window.h src/gui/rectangle.cpp src/gui/rectangle.h src/gui/compass_marker.cpp src/gui/compass_marker.h)
target_link_libraries(main ppgso shaders ${OpenMP_libomp_LIBRARY})
install (TARGETS main DESTINATION .)

#
# INSTALLATION
#
file(COPY "data/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(DIRECTORY data/ DESTINATION .)
