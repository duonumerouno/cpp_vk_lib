include_directories(../lib/include)
include_directories(../dependencies/simdjson/include)
include_directories(../dependencies)

set(SOURCES
    main.cpp
)
add_executable(
    worker ${SOURCES}
)

target_link_libraries(worker -Lshared cpp_vk_lib pthread curl curlpp -L../dependencies/simdjson simdjson)
