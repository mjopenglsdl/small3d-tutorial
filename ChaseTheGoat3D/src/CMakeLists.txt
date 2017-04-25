ADD_EXECUTABLE(chasethegoat3d main.cpp GameLogic.cpp)

# Need to find GLM using the packaged FindGLM module because of non-standard include directory (glm/glm instead of include/glm)
find_package(GLM REQUIRED)

TARGET_INCLUDE_DIRECTORIES(chasethegoat3d PUBLIC "${game_SOURCE_DIR}/include" "${GLM_INCLUDE_DIRS}")

TARGET_LINK_LIBRARIES(chasethegoat3d PUBLIC "${CONAN_LIBS}")

SET_TARGET_PROPERTIES(chasethegoat3d PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

IF(WIN32)
  SET_TARGET_PROPERTIES(chasethegoat3d PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:MSVCRTD")
ENDIF()
