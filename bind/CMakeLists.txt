cmake_minimum_required(VERSION 3.20.0)
project(bind)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(bind main.cpp)
target_compile_options(bind PRIVATE -O3)
target_include_directories(bind PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(bind PRIVATE ${OpenCV_LIBRARIES})
