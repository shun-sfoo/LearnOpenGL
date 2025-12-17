cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)
set(CMAKE_BUILD_TYPE Release)
set(GLFW_BUILD_WAYLAND ON)
set(GLFW_BUILD_X11 OFF)

project(LearnOpenGL)

option(USER_BABY "Enable baby mode" OFF)

include(FetchContent)

include(${PROJECT_SOURCE_DIR}/cmake/glad_v1.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/glfw.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/glm.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/imgui.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/stb.cmake)

find_package(fmt CONFIG)
find_package(TBB CONFIG)

add_subdirectory(v1/breakout)
add_executable(triangle v1/triangle.cpp)
target_link_libraries(triangle PRIVATE imgui)
target_include_directories(triangle PRIVATE ${CMAKE_BINARY_DIR}/generated/)

if(fmt_FOUND)
  target_link_libraries(triangle PRIVATE fmt::fmt)
endif()

if(TBB_FOUND)
  target_link_libraries(triangle PRIVATE TBB:tbb)
endif()

# add_executable(app main.cpp)

# target_link_libraries(
#         app
#         imgui
#         glm
#         stb
# )

configure_file(config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)
