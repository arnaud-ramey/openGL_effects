project(openGL_effects)
cmake_minimum_required(VERSION 2.8)
# Debug, Release, RelWithDebInfo and MinSizeRe
set(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # add extra warnings

find_package(GLUT REQUIRED)
#~ find_package(JPEG REQUIRED)
#~ find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
#~ find_package(Threads REQUIRED)
include_directories(libs)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/openGL_effects_path.h.in"
               "${PROJECT_BINARY_DIR}/openGL_effects_path.h")

add_executable(test_openGL_effects
  libs/bitmap/Bitmap.cpp
  libs/rtg/RTGlib.c
  main.cpp)
target_link_libraries(test_openGL_effects
  ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
