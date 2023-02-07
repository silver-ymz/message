set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets WebSockets)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.qrc src/*.ui)
add_executable(${PROJECT_NAME}_app ${SOURCES})
target_link_libraries(${PROJECT_NAME}_app PRIVATE Qt::Widgets Qt::WebSockets)

if(APPLE)
  set_target_properties(${PROJECT_NAME}_app PROPERTIES MACOSX_BUNDLE TRUE)
elseif(WIN32)
  set_target_properties(${PROJECT_NAME}_app PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
