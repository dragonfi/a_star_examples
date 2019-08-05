cmake_minimum_required (VERSION 3.10)

project (AStar)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

SET(inc "")
SET(lib "")

find_package(SDL2 REQUIRED)
SET(inc ${inc} ${SDL2_INCLUDE_DIRS})
SET(lib ${lib} ${SDL2_LIBRARIES})

find_package(OpenGL REQUIRED)
SET(inc ${inc} ${OPENGL_INCLUDE_DIR})
SET(lib ${lib} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
SET(inc ${inc} ${GLEW_INCLUDE_DIR})
SET(lib ${lib} ${GLEW_LIBRARIES})

#find_package(SDL2_image REQUIRED)
#SET(inc ${inc} ${SDL_IMAGE_INCLUDE_DIR})
#SET(lib ${lib} ${SDL2_IMAGE_LIBRARIES})

message(STATUS "inc: ${inc}")
message(STATUS "lib: ${lib}")

add_executable(AStar src/main.cpp ${sources})
target_include_directories(AStar PUBLIC src/ ${inc})
target_link_libraries(AStar ${lib})


SET(CMAKE_CXX_COMPILER "g++")

SET(profile_flags "-pg")
SET(optimization_flags "-O3")
SET(warning_flags "-Wall -Werror -Wfatal-errors -pedantic")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warning_flags} ${optimization_flags} ${profile_flags}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${profile_flags}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${profile_flags}")
