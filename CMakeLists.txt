cmake_minimum_required(VERSION 3.14)
project(math)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_subdirectory(src)

add_executable(asc main.cpp)
target_link_libraries(asc math)