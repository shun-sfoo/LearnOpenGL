cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

project(LearnOpenGL)

include(FetchContent)

include(${PROJECT_SOURCE_DIR}/cmake/glad.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/glfw.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/glm.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/imgui.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/stb.cmake)

add_executable(app main.cpp)

target_link_libraries(
        app
        imgui
        glm
        stb
)
