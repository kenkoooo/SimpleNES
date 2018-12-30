cmake_minimum_required(VERSION 3.1)
project(SimpleNES)
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Add directory containing FindSFML.cmake to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")
include_directories(${SFML_INCLUDE_DIR})
find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)

include_directories(./include/)
add_subdirectory(src)
add_executable(SimpleNES main.cpp)
target_link_libraries(SimpleNES Controller Emulator Log)
target_link_libraries(SimpleNES ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
