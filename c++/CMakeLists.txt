cmake_minimum_required (VERSION 3.10)
project (AStar)
add_executable(FancyImplementationAStar fancyImplementationMain.cpp)

add_executable(AStar main.cpp)

# subdirectories
#add_subdirectory (Hello)
#add_subdirectory (Demo)

# Hello directory
#add_library (Hello hello.cxx)
#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Demo directory
#add_executable (helloDemo demo.cxx demo_b.cxx)
#target_link_libraries (helloDemo LINK_PUBLIC Hello)

SET(CMAKE_CXX_COMPILER "clang++")

SET(PROFILE_FLAGS "-pg")
SET(OPTIMIZATION_FLAGS "-O3")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror ${OPTIMIZATION_FLAGS} ${PROFILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PROFILE_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${PROFILE_FLAGS}")

add_definitions(-Wfatal-errors)
