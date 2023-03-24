cmake_minimum_required(VERSION 3.14)
project(IoTPayload)

add_library (payload STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/payload.cpp
)
target_include_directories(payload
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(testing)
