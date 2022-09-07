cmake_minimum_required(VERSION 3.10)

project("SoftRender_CMake_MSVC" VERSION 0.1.0)

if(NOT MSVC)
    message(FATAL_ERROR "Can only be compiled with MSVC!")
endif()

add_compile_options(
    /utf-8
    /W4
)

message(${CMAKE_CXX_FLAGS})

add_definitions(-DUNICODE -D_UNICODE)

add_executable(main
    "src/main.h" "src/main.cpp"

    "src/core/window.h" "src/core/window.cpp"
    "src/core/canvas.h" "src/core/canvas.cpp"
    "src/core/render.h" "src/core/render.cpp"

    "src/app/my_window.h" "src/app/my_window.cpp"
)

target_include_directories(main PRIVATE
    "include"
    "3rdParty/SDL2-2.24.0/include"
)
target_link_directories(main PRIVATE
    "lib"
    "3rdParty/SDL2-2.24.0/lib/x64"
)
target_link_libraries(main PRIVATE
    SDL2
    SDL2main
    SDL2test
)

set_target_properties(main
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)
