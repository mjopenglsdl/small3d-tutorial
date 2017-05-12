CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)
PROJECT(demo)

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

add_executable(ball main.cpp)

target_link_libraries(ball PUBLIC "${CONAN_LIBS}")
set_target_properties(ball PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")
