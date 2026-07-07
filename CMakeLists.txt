cmake_minimum_required(VERSION 3.20)
project(SquidGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib already compiled - link directly
set(RAYLIB_INCLUDE_DIR "C:/raylib/w64devkit/include")
set(RAYLIB_LIB_DIR     "C:/raylib/w64devkit/lib")

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${RAYLIB_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${RAYLIB_LIB_DIR}/libraylib.a
    opengl32
    gdi32
    winmm
)