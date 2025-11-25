cmake_minimum_required(VERSION 3.10)
project(rubiks)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/glad.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
