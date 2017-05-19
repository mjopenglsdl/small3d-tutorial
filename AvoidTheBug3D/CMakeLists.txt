cmake_minimum_required(VERSION 3.8.1)
project(game)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${game_BINARY_DIR}/bin")
  if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${game_BINARY_DIR}/bin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${game_BINARY_DIR}/bin")
  endif(MSVC)

  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${game_BINARY_DIR}/lib")
  if(MSVC)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${game_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${game_BINARY_DIR}/lib")
  endif(MSVC)

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${game_BINARY_DIR}/lib")
  if(MSVC)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${game_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${game_BINARY_DIR}/lib")
  endif(MSVC)


set(DEPS_PATH "${CMAKE_SOURCE_DIR}/deps")
set(CMAKE_PREFIX_PATH ${DEPS_PATH})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/deps/cmake")

find_package(SMALL3D REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)
find_package(GLM)
find_package(OGG REQUIRED)
find_package(VORBIS REQUIRED)
find_package(Portaudio REQUIRED)
find_package(Freetype REQUIRED)
if(NOT WIN32)
  find_package(BZip2 REQUIRED)
endif()
find_package(PNG REQUIRED)
find_package(OGG REQUIRED)

subdirs(src)

if(WIN32)
  file(COPY "deps/bin" DESTINATION "${PROJECT_BINARY_DIR}")
endif(WIN32)

file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")
