add_executable(launcher src/main.c
        ../library/include/bzf_error_flag.h)

# Link the library
target_link_libraries(launcher PRIVATE bzf_http_server)
