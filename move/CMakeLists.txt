cmake_minimum_required(VERSION 3.20.0)
project(move)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(move main.cpp)
target_compile_options(move PRIVATE -O3)
target_include_directories(move PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(move PRIVATE ${OpenCV_LIBRARIES})
