cmake_minimum_required(VERSION 3.17)
project(HashMasm)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(HashMasmTests  tests/basicTests.cpp tests/basicTests.cpp tests/testTools.h)
target_link_libraries(HashMasmTests gtest gtest_main)

include_directories(src/CppSwiftyList)
add_library(HashMasmLib SHARED src/HashMasm.h src/HashMasm.cpp src/HashMasm.cpp src/CRC.h src/CRCLookup.h)

add_executable(HashMasmTranslate examples/main.cpp)
target_link_libraries(HashMasmTranslate HashMasmLib)