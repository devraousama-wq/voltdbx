cmake_minimum_required(VERSION 3.16)
project(voltdbx VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(VOLTDBX_BUILD_TESTS "Build unit tests" ON)

set(VOLTDBX_SOURCES
    src/main.cpp
    src/config.cpp
    src/server/database_server.cpp
    src/net/tcp_server.cpp
    src/util/logger.cpp
    src/storage/storage_engine.cpp
    src/protocol/parser.cpp
)

add_executable(voltdbx ${VOLTDBX_SOURCES})

target_include_directories(voltdbx PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(VOLTDBX_BUILD_TESTS)
    enable_testing()
endif()

install(TARGETS voltdbx RUNTIME DESTINATION bin)
