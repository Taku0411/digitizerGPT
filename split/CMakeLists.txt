cmake_minimum_required(VERSION 3.20.0)
project(split)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(split main.cpp)
target_compile_options(split PRIVATE -O3)
target_include_directories(split PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(split PRIVATE ${OpenCV_LIBRARIES})
