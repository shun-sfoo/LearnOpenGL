include(FetchContent)
cmake_minimum_required(VERSION 3.17)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置默认构建类型（如果未指定）
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "选择构建类型" FORCE)
endif()

# 根据构建类型设置BUILD_TEST
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()

FetchContent_Declare(
        imgui-glfw-glad-glm
        GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
)
FetchContent_MakeAvailable(imgui-glfw-glad-glm)

FetchContent_Declare(
        imgui-glfw-glad-glm
        GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
)

set(ASSIMP_BUILD_TESTS OFF)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        v6.0.1          # 建议使用明确的发布版本标签
)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
)

FetchContent_MakeAvailable(assimp stb)

message("CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
message("ASSIMP_INSTALL ${ASSIMP_INSTALL}")
message("ASSIMP_BUILD_TESTS ${ASSIMP_BUILD_TESTS}")

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

add_executable(app main.cpp)

target_link_libraries(
        app
        imgui
        glm
        stb
)
