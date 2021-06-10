cmake_minimum_required(VERSION 3.16)
project(cg)

set(CMAKE_CXX_STANDARD 20)

add_executable(cg main.cpp
        glut_ply.h glut_ply.cpp
        matriz.cpp matriz.h )

include_directories(/usr/local/include)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(cg ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
