cmake_minimum_required(VERSION 3.12)

project(Kolik CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed!")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(KOLIK_INC ${CMAKE_SOURCE_DIR}/Kolik)

add_subdirectory(Kolik)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(REPROC++ ON)

add_subdirectory(vendor/reproc)
add_subdirectory(vendor/nlohmann)