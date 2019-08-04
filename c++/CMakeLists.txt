cmake_minimum_required (VERSION 3.10)

project (AStar)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

add_executable(AStar src/main.cpp ${sources})

SET(CMAKE_CXX_COMPILER "clang++")

SET(profile_flags "-pg")
SET(optimization_flags "-O3")
SET(warning_flags "-Wall -Werror -Wfatal-errors -pedantic")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warning_flags} ${optimization_flags} ${profile_flags}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${profile_flags}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${profile_flags}")
