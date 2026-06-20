# Custom helper to add a cmocka-based test executable.
# We intentionally do NOT name it add_cmocka_test to avoid clashing with the
# function of the same name shipped inside cmocka (AddCMockaTest.cmake).
#
# Every test binary is automatically linked with the mock OS sources so that
# OS symbols (bzf_os_malloc, bzf_os_free, etc.) are resolved at link time.
function(bzf_add_cmocka_test TEST_NAME TEST_SOURCE LIB_TARGET)
    add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE})
    target_sources(${TEST_NAME} PRIVATE ${BZF_OS_MOCK_SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE cmocka ${LIB_TARGET} bzf_os_headers bzf_os_mocks_headers)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()
