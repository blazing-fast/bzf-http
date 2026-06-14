add_executable(launcher
    src/main.c
    src/coucou_endpoint.c
    src/root_endpoint.c
)

# Link the library
target_link_libraries(launcher PRIVATE bzf_http_server)
