# Custom helper to add a cmocka-based test executable.
# We intentionally do NOT name it add_cmocka_test to avoid clashing with the
# function of the same name shipped inside cmocka (AddCMockaTest.cmake).
#
# Every test binary is automatically linked with bzf_os_mock so that OS symbols
# (bzf_os_malloc, bzf_os_free, etc.) are mockable at link time.
#
# Optional MOCK_TARGETS argument: list of OBJECT-library mock targets whose
# compiled objects are injected BEFORE the real static libraries. This lets
# individual symbols be overridden at link time with zero production overhead.
function(bzf_add_cmocka_test TEST_NAME TEST_SOURCE LIB_TARGET)
    cmake_parse_arguments(PARSE_ARGV 3 "ARG" "" "" "MOCK_TARGETS")

    add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SOURCE})

    # Link mock object targets first so their symbols take precedence over the
    # real ones inside the static libraries (CMake >= 3.12 supports linking
    # OBJECT libraries directly).
    foreach(MOCK ${ARG_MOCK_TARGETS})
        target_link_libraries(${TEST_NAME} PRIVATE ${MOCK})
    endforeach()

    target_link_libraries(${TEST_NAME} PRIVATE
        cmocka
        ${LIB_TARGET}
        bzf_os_mock
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()
