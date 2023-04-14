cmake_minimum_required(VERSION 3.20.0)
project(crop)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(crop main.cpp)
target_compile_options(crop PRIVATE -O3)
target_include_directories(crop PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(crop PRIVATE ${OpenCV_LIBRARIES})