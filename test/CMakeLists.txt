add_executable( camera_test camera_test.cpp )
target_include_directories(camera_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(camera_test raylib)
add_test( NAME camera COMMAND camera_test )

add_executable( scene_test scene_test.cpp )
target_include_directories(scene_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(scene_test raylib)
add_test( NAME scene COMMAND scene_test )
