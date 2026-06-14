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


# --- Add subdirectories ---
add_subdirectory(libs/bytes)
add_subdirectory(libs/hashmap)
add_subdirectory(libs/dynamic_array)
add_subdirectory(libs/http_server)
add_subdirectory(launcher)
