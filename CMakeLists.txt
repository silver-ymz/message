cmake_minimum_required(VERSION 3.14)
project(message LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXES ${PROJECT_NAME}_app ${PROJECT_NAME}_server)

add_subdirectory(frontend)
add_subdirectory(backend)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER_CASE)
if(result AND CMAKE_BUILD_TYPE_LOWER_CASE STREQUAL "release")
  foreach(EXE ${EXES})
    set_target_properties(${EXE} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
  endforeach()
endif()
