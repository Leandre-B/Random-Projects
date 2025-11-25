cmake_minimum_required(VERSION 3.18)
project(labyrinthe)

# Vérifie que SFML est installé :
find_package(SFML COMPONENTS graphics window system)

add_compile_options(-Wall)

add_executable(${PROJECT_NAME}
  sources/main.cpp
  sources/gen_laby.cpp
  sources/draw.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
