cmake_minimum_required(VERSION 3.20.0)
project(rotate)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(rotate main.cpp)
target_compile_options(rotate PRIVATE -O3)
target_include_directories(rotate PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(rotate PRIVATE ${OpenCV_LIBRARIES})
