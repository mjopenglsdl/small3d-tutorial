add_executable(chasethegoat3d main.cpp GameLogic.cpp)

target_include_directories(chasethegoat3d PUBLIC "${game_SOURCE_DIR}/include")

target_link_libraries(chasethegoat3d PUBLIC "${CONAN_LIBS}")

set_target_properties(chasethegoat3d PROPERTIES LINK_FLAGS "${CONAN_EXE_LINKER_FLAGS}")

if(MSVC)
  set_target_properties(chasethegoat3d PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:MSVCRTD")
  set_target_properties(chasethegoat3d PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${game_BINARY_DIR}/bin")
endif()
