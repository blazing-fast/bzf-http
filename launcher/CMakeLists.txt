add_executable(launcher
    src/main.c
    src/coucou_endpoint.c
    src/root_endpoint.c
)

# OS symbols are now brought in transitively by bzf_http_server -> bzf_os.
target_link_libraries(launcher PRIVATE bzf_http_server)
