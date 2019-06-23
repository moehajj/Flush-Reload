cmake_minimum_required(VERSION 3.5)
project(flush+reload)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (libs)

add_executable (sender sender.c)
target_link_libraries (sender LINK_PUBLIC util)

add_executable (receiver receiver.c)
target_link_libraries (receiver LINK_PUBLIC util)

