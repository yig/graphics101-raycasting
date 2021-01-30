cmake_minimum_required(VERSION 3.2)

project(raycasting)

# We want C++11
set (CMAKE_CXX_STANDARD 11)

set(SRCS
    src/camera.cpp
    src/camera.h
    src/debugging.h
    src/image.cpp
    src/image.h
    src/light.h
    src/main.cpp
    src/material.h
    src/mesh.cpp
    src/parser.cpp
    src/scene.cpp
    src/scene.h
    src/shape.cpp
    src/shape.h
    src/stb_image.h
    src/stb_image_resize.h
    src/stb_image_write.h
    src/types.h
    )

add_executable(raycasting ${SRCS})
target_include_directories(raycasting PUBLIC include)
target_link_libraries(raycasting)

# Create the output directory and remember its location.
set(OUTPUT "${PROJECT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY "${OUTPUT}")

include("CMakeLists-zip.txt" OPTIONAL)
