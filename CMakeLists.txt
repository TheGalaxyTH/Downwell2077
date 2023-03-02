cmake_minimum_required(VERSION 3.23)
project(Downwell)

set(CMAKE_C_STANDARD 11)

add_executable(downwell downwell.c)
target_link_libraries(downwell mingw32)
target_link_libraries(downwell SDL2main)
target_link_libraries(downwell SDL2)
target_link_libraries(downwell SDL2_image)
target_link_libraries(downwell SDL2_ttf)
target_link_libraries(downwell SDL2_mixer)
