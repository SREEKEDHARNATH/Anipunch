cmake_minimum_required(VERSION 3.20)
project(p LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SDL2 STATIC IMPORTED)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2.a"
)

add_library(SDL2_IMAGE STATIC IMPORTED)
set_target_properties(SDL2_IMAGE PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2_image.a"
)

add_library(FREETYPE STATIC IMPORTED)
set_target_properties(FREETYPE PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libfreetype.a"
)

add_library(SDL2_TTF STATIC IMPORTED)
set_target_properties(SDL2_TTF PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2_ttf.a"
    INTERFACE_LINK_LIBRARIES "FREETYPE;SDL2"
)

add_library(SDL2_MIXER STATIC IMPORTED)
set_target_properties(SDL2_MIXER PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libSDL2_mixer.a"
)

add_library(box2d STATIC IMPORTED)
set_target_properties(box2d PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/libbox2d.a"
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets SYMBOLIC)

add_executable(p ${SOURCES})

target_include_directories(p PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_options(p PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(p PRIVATE
    m
    box2d
    SDL2
    SDL2_IMAGE
    FREETYPE
    SDL2_TTF
    SDL2_MIXER
)
