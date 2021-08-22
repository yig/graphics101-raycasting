cmake_minimum_required(VERSION 3.14)

project(raycasting LANGUAGES CXX C)

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

include(cmake/CPM.cmake)
CPMAddPackage("https://github.com/g-truc/glm.git#0.9.9.8")
## This is a single header file. Let's just put it in our "include" folder directly.
# CPMAddPackage("https://github.com/nlohmann/json.git@3.9.1")

add_executable(raycasting ${SRCS})
target_include_directories(raycasting PUBLIC include)
## On Windows, the following line may help build a static binary that can be called from the autograder:
# target_link_libraries(raycasting -static)
## Some platforms can't build entirely static binaries, so I'm leaving it off by default.
target_link_libraries(raycasting glm::glm)

# Create the output directory and remember its location.
set(OUTPUT "${PROJECT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY "${OUTPUT}")

include("CMakeLists-zip.txt" OPTIONAL)
