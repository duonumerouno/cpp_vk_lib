include_directories(include)
include_directories(../dependencies)

add_definitions(-DVK_CURL_DEBUG)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -msse3 -mtune=generic -march=native")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -msse3 -march=i686 -mtune=generic")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")
endif()

add_library(
    cpp_vk_lib
    SHARED
    main.cpp
    src/api/long_poll_api.cpp
    src/api/base_object.cpp
    src/attachment/attachment.cpp
    src/document/common_document.cpp
    src/events/common_event.cpp
    src/events/message_new.cpp
    src/events/wall_post_new.cpp
    src/methods/audio.cpp
    src/methods/docs.cpp
    src/methods/photos.cpp
    src/methods/video.cpp
    src/methods/messages.cpp
    src/net/network_client.cpp
    src/processing/thread_pool.cpp
    ../dependencies/logger/logger.cpp
)
target_link_libraries(
    cpp_vk_lib
    avformat simdjson
)
