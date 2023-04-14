cmake_minimum_required(VERSION 3.20.0)
project(deshadow_middle)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(deshadow_middle main.cpp)
target_compile_options(deshadow_middle PRIVATE -O3)
target_include_directories(deshadow_middle PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(deshadow_middle PRIVATE ${OpenCV_LIBRARIES})
