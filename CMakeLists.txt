cmake_minimum_required(VERSION 3.16)

project(elhaylib_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include headers
include_directories(${PROJECT_SOURCE_DIR})

# Build the library
add_library(elhaylib STATIC
    elhaylib.c
)

# Compiler warnings
if(MSVC)
    target_compile_options(elhaylib PRIVATE /W4 /WX)
else()
    target_compile_options(elhaylib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Enable testing
enable_testing()

# Build all test files in tests/ folder
file(GLOB TEST_SOURCES "tests/*.c")

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE elhaylib)

    if(MSVC)
        target_compile_options(${TEST_NAME} PRIVATE /W4 /WX)
    else()
        target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    # Register test with CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
