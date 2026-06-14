add_library(bzf_dynamic_array STATIC
    src/bzf_dynamic_array.c
    include/bzf_dynamic_array.h
    src/bzf_dynamic_array_impl.h
)

target_include_directories(bzf_dynamic_array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(bzf_dynamic_array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bzf_dynamic_array PUBLIC bzf_bytes)
