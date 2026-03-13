cmake_minimum_required(VERSION 3.10)
project(raytrace VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_directories(${PROJECT_NAME} PRIVATE)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_options(${PROJECT_NAME} PUBLIC -Wextra -Werror -Wall -Wpedantic)
target_link_libraries(${PROJECT_NAME} raylib m pthread dl)
