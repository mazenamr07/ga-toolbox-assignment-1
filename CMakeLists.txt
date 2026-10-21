cmake_minimum_required(VERSION 3.16)

project(GAToolboxAssignment1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ga_toolbox
    src/GAConfig.cpp
    src/GAEngine.cpp
)

target_include_directories(ga_toolbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(ga_toolbox PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

add_executable(member1_smoke_test
    tests/member1_smoke_test.cpp
)

target_link_libraries(member1_smoke_test PRIVATE ga_toolbox)
