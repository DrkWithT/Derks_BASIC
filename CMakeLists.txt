cmake_minimum_required(VERSION 3.30)

project(derkbasic VERSION 0.0.1 LANGUAGES C)

# set language configs.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# configure build type
set(DO_DEBUG_MODE ON CACHE BOOL "Building mode: debug or release")

if (${DO_DEBUG_MODE})
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -Og)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
endif ()

# path config
set(PROJ_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes")
set(PROJ_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJ_LIBS_DIR "${CMAKE_SOURCE_DIR}/build")
set(PROJ_TEST_CODE_DIR "${CMAKE_SOURCE_DIR}/test_code")

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
