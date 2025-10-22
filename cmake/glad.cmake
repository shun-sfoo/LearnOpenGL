set(GLAD_GL_PROFILE core)
set(GLAD_GL_VERSION 4.6 CACHE STRING "The target gl version")

find_program(LEPT_CURL NAMES curl curl.exe)
if(NOT LEPT_CURL)
  message(STATUS "Could not find curl, using included version of the glad loader sources (gl 4.5/core)")
  set(glad_SOURCE_DIR glad)
else()
  execute_process(
    COMMAND ${LEPT_CURL} -s -D - -X POST -d generator=c&api=egl%3Dnone&api=gl%3D${GLAD_GL_VERSION}&profile=gl%3D${GLAD_GL_PROFILE}&api=gles1%3Dnone&profile=gles1%3Dcommon&api=gles2%3Dnone&api=glsc2%3Dnone&api=glx%3Dnone&api=vulkan%3Dnone&api=wgl%3Dnone&options=LOADER https://gen.glad.sh/generate
    OUTPUT_VARIABLE out
    RESULT_VARIABLE res
    ERROR_VARIABLE err)

  if(NOT res EQUAL "0")
    message(WARNING "${LEPT_CURL} returned: " ${res})
    if(err)
      message(WARNING "Error message: " ${err})
    endif()
    message(STATUS "Using included version of the glad loader sources (gl 4.6/core)")
    set(glad_SOURCE_DIR glad)
  else()
    string(REGEX MATCH "[Ll][Oo][Cc][Aa][Tt][Ii][Oo][Nn]: ([A-Za-z0-9_\\:/\\.]+)" location ${out})
    set(location "${CMAKE_MATCH_1}")
    if(NOT location OR location STREQUAL "/")
      message(WARNING "Could not extract location from http response, using included version of the glad loader sources (gl 4.5/core)")
      message(STATUS "Response: " ${out})
      set(glad_SOURCE_DIR glad)
    else()
      string(REGEX REPLACE "/$" "" location ${location})
      string(APPEND location "/glad.zip")
      if(NOT ${location} MATCHES "^http")
        string(PREPEND location "https://gen.glad.sh")
      endif()
      message(STATUS "Downloading glad loader sources for gl${GLAD_GL_VERSION}/${GLAD_GL_PROFILE} from ${location}")

      FetchContent_Declare(
        glad
        URL ${location}
      )
      FetchContent_MakeAvailable(glad)

      set(glad_INSTALLED_VERSION ${GLAD_GL_VERSION}-${GLAD_GL_PROFILE} CACHE INTERNAL "")
      set(glad_LAST_SOURCE_DIR ${glad_SOURCE_DIR} CACHE INTERNAL "")
    endif()
  endif()
endif()

add_library(
  glad
  ${glad_SOURCE_DIR}/src/gl.c
  ${glad_SOURCE_DIR}/include/glad/gl.h
  ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
