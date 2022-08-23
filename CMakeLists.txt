cmake_minimum_required(VERSION 3.8)

project("OpenGL_CMake_MSVC" VERSION 0.1.0)

if(NOT MSVC)
    message(FATAL_ERROR "Can only be compiled with MSVC!")
endif()

add_compile_options("/utf-8")

add_executable(main
    "src/TheMain.h" "src/TheMain.cpp"
    "src/util/render.h" "src/util/render.cpp"
)

target_include_directories(main PRIVATE include)
# target_link_directories(main PRIVATE lib)
# target_link_libraries(main PRIVATE glfw3dll)

set_target_properties(main
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)
