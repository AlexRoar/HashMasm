cmake_minimum_required(VERSION 3.17)
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")
add_compile_options(
        "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
    $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)

project(HashMasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g -masm=intel -msse4.2 -mavx -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -masm=intel -msse4.2 -mavx -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -masm=intel")
set(CMAKE_ASM_NASM_FLAGS_DEBUG "${CMAKE_ASM_NASM_FLAGS} -f macho64 -g -F dwarf -L asm.lst")
set(CMAKE_ASM_NASM_FLAGS_RELEASE "${CMAKE_ASM_NASM_FLAGS_RELEASE} -f macho64 -g -F dwarf -L asm.lst")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(HashMasmLibAsm OBJECT src/optimizations.asm)
set_target_properties(HashMasmLibAsm PROPERTIES NASM_OBJ_FORMAT macho64)

include_directories(src)
add_library(HashMasmLib SHARED src/hash/hashes.cpp src/HashMasm.cpp  src/HashMasmFixed.cpp src/hash/hashes.cpp src/helpers/leoTestsHelpers.cpp)
target_link_libraries(HashMasmLib HashMasmLibAsm)
target_include_directories(HashMasmLib PRIVATE src)
set_target_properties(HashMasmLib PROPERTIES PUBLIC_HEADER src/HashMasm.h)

add_executable(HashMasmTests tests/basicTests.cpp)
add_executable(HashMasmComp  examples/comparison.cpp)
add_executable(HashMasmLeo   examples/leoTests.cpp)
add_executable(HashMasm      examples/leo.cpp)
add_executable(HashMasmDict  examples/dict.cpp)
add_executable(HashMasmLFT   examples/LFTests.cpp)

target_link_libraries(HashMasmLeo   HashMasmLib HashMasmLibAsm)
target_link_libraries(HashMasmComp  HashMasmLib HashMasmLibAsm)
target_link_libraries(HashMasm      HashMasmLib HashMasmLibAsm)
target_link_libraries(HashMasmTests HashMasmLib HashMasmLibAsm)
target_link_libraries(HashMasmTests gtest gtest_main )
target_link_libraries(HashMasmDict  HashMasmLib HashMasmLibAsm)
target_link_libraries(HashMasmLFT   HashMasmLib HashMasmLibAsm)
