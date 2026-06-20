add_executable(launcher
    src/main.c
    src/coucou_endpoint.c
    src/root_endpoint.c
)

# Link the library + real OS sources for production.
target_sources(launcher PRIVATE ${BZF_OS_REAL_SOURCES})
target_link_libraries(launcher PRIVATE bzf_http_server)
