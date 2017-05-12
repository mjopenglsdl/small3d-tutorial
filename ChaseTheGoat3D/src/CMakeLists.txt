add_executable(chasethegoat3d main.cpp GameLogic.cpp)

target_include_directories(chasethegoat3d PUBLIC "${game_SOURCE_DIR}/include")

target_link_libraries(chasethegoat3d PUBLIC "${CONAN_LIBS}")

set_target_properties(chasethegoat3d PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

if(WIN32)
  set_target_properties(chasethegoat3d PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:MSVCRTD")
endif()
