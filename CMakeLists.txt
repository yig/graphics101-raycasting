cmake_minimum_required(VERSION 3.1.0)

project(raycasting)

# We want C++17 for filesystem
set (CMAKE_CXX_STANDARD 17)

add_executable(raycasting
    camera.cpp
    image.cpp
    main.cpp
    mesh.cpp
    scene.cpp
    shape.cpp
    parser.cpp
)

target_link_libraries(raycasting)
