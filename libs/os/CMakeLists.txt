# bzf_os — OS wrapper library.
# Production binaries link the STATIC library.
# Test binaries link the OBJECT mock library to override specific OS calls.

file(GLOB BZF_OS_REAL_SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB BZF_OS_MOCK_SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.c")

add_library(bzf_os STATIC ${BZF_OS_REAL_SOURCES_LIST})
target_include_directories(bzf_os PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(bzf_os_mock OBJECT ${BZF_OS_MOCK_SOURCES_LIST})
target_include_directories(bzf_os_mock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
target_include_directories(bzf_os_mock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
