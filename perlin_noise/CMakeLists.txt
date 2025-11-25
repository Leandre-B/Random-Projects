cmake_minimum_required(VERSION 3.25)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#import la bibliothèque SFML depuis un repo github
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#importe la bibliothèque SFML depuis le repo github SFML
add_executable(sokoban 
    src/perlin.cpp
)

target_link_libraries(sokoban PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(sokoban PRIVATE cxx_std_17)

#copie les dlls à côter des l'executable (pour Windows)
if(WIN32)
    add_custom_command(
        TARGET sokoban
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sokoban>
        VERBATIM)
endif()