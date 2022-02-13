cmake_minimum_required(VERSION 3.0)

project(MarchingSquares LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# build
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE ${ROOT}/include)
set(SRC ${ROOT}/src)
set(BIN ${ROOT}/bin)
set(DEPS ${ROOT}/deps)

set(EXECUTABLE_OUTPUT_PATH ${BIN})

file(GLOB_RECURSE sources "${INCLUDE}/*.h" "${SRC}/*.cpp")
add_executable(MarchingSquares ${sources})

target_include_directories(MarchingSquares PRIVATE ${INCLUDE})
target_link_libraries(MarchingSquares PRIVATE glfw glad)

# dependencies
add_subdirectory(${DEPS}/glfw)
add_subdirectory(${DEPS}/glad)