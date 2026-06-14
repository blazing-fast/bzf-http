cmake_minimum_required(VERSION 3.15)
project(Tasks C)

# --- C Standard ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# --- Compiler warnings ---
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wvla -Werror=vla -Wall -Wextra -Wpedantic)
endif()

# --- Enable sanitizers in Debug mode ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling ASan + UBSan")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# --- Testing ---
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    include(CMockaTest)

    # Prefer system cmocka (e.g. from Docker's libcmocka-dev),
    # fallback to FetchContent on systems without it (e.g. macOS).
    find_package(cmocka QUIET)
    if(NOT cmocka_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            cmocka
            GIT_REPOSITORY https://gitlab.com/cmocka/cmocka.git
            GIT_TAG        cmocka-1.1.7
            GIT_SHALLOW    TRUE
        )
        set(WITH_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(WITH_UNIT_TESTS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(cmocka)

        # Make cmocka headers findable for test sources
        include_directories(${cmocka_SOURCE_DIR}/include)
    endif()

    add_subdirectory(tests)
endif()

# --- Add subdirectories ---
add_subdirectory(libs/bytes)
add_subdirectory(libs/hashmap)
add_subdirectory(libs/dynamic_array)
add_subdirectory(libs/http_server)
add_subdirectory(launcher)
