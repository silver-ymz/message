find_package(Boost REQUIRED COMPONENTS system thread)
find_package(fmt REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME}_server ${SOURCES})
target_link_libraries(${PROJECT_NAME}_server PRIVATE ${Boost_LIBRARIES}
                                                     fmt::fmt)
