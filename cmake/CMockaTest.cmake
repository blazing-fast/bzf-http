# Custom helper to add a cmocka-based test executable.
# We intentionally do NOT name it add_cmocka_test to avoid clashing with the
# function of the same name shipped inside cmocka (AddCMockaTest.cmake).
function(bzf_add_cmocka_test TEST_NAME TEST_SOURCE LIB_TARGET)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE cmocka ${LIB_TARGET})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()
