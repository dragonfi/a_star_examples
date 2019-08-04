cmake_minimum_required (VERSION 3.10)

project (AStar)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

#add_library(vec2 STATIC src/vec2.cpp)
#add_library(randomPoints STATIC src/randomPoints.cpp)
#add_library(graph STATIC src/graph.cpp)
add_executable(AStar src/main.cpp ${sources})
#target_link_libraries(AStar vec2 randomPoints graph)

SET(CMAKE_CXX_COMPILER "clang++")

SET(profile_flags "-pg")
SET(optimization_flags "-O3")
SET(warning_flags "-Wall -Werror -Wfatal-errors -pedantic")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warning_flags} ${optimization_flags} ${profile_flags}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${profile_flags}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${profile_flags}")
