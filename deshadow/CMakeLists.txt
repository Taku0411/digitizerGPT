cmake_minimum_required(VERSION 3.20.0)
project(deshadow)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(deshadow main.cpp)
target_compile_options(deshadow PRIVATE -O3)
target_include_directories(deshadow PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(deshadow PRIVATE ${OpenCV_LIBRARIES})
