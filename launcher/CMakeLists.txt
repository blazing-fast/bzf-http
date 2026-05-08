add_executable(launcher src/main.c
)

# Link the library
target_link_libraries(launcher PRIVATE bzf_http_server)
