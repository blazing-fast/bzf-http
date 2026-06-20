# bzf_os is NOT compiled as a library.
# It provides two source lists consumed by test and production binaries.

file(GLOB BZF_OS_REAL_SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB BZF_OS_MOCK_SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.c")

# Expose as PARENT_SCOPE so every subdirectory can use them.
set(BZF_OS_REAL_SOURCES ${BZF_OS_REAL_SOURCES_LIST} PARENT_SCOPE)
set(BZF_OS_MOCK_SOURCES ${BZF_OS_MOCK_SOURCES_LIST} PARENT_SCOPE)

# Consumers still need the OS headers.
add_library(bzf_os_headers INTERFACE)
target_include_directories(bzf_os_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Mocks header — test targets link this to #include "bzf_os_mocks.h"
add_library(bzf_os_mocks_headers INTERFACE)
target_include_directories(bzf_os_mocks_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
