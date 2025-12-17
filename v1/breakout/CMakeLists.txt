file(GLOB_RECURSE srcs CONFIGURE_DEPENDS include/*.h src/*.cpp)
add_library(breakout STATIC ${srcs})

target_include_directories(breakout PUBLIC include ${CMAKE_BINARY_DIR}/generated/)

target_link_libraries(breakout PUBLIC imgui glm stb)
