cmake_minimum_required (VERSION 3.10)
project (AStar)
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

add_definitions(-Wfatal-errors)