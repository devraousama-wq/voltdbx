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
    src/server/command_handler.cpp
    src/net/tcp_server.cpp
    src/net/client_session.cpp
    src/net/accept_loop.cpp
    src/net/io_multiplexer.cpp
    src/net/async_accept_loop.cpp
    src/net/connection_pool.cpp
    src/util/logger.cpp
    src/storage/storage_engine.cpp
    src/storage/hash_table.cpp
    src/memory/buffer_pool.cpp
    src/memory/string_pool.cpp
    src/persistence/snapshot.cpp
    src/persistence/snapshot_scheduler.cpp
    src/persistence/aof_log.cpp
    src/storage/storage_hooks.cpp
    src/ttl/expiration.cpp
    src/ttl/scheduler.cpp
    src/pubsub/channel_registry.cpp
    src/pubsub/broker.cpp
    src/pubsub/session_mailbox.cpp
    src/concurrency/thread_pool.cpp
    src/concurrency/session_worker.cpp
    src/protocol/parser.cpp
    src/protocol/command.cpp
    src/protocol/dispatcher.cpp
    src/protocol/response.cpp
)

add_executable(voltdbx ${VOLTDBX_SOURCES})

target_include_directories(voltdbx PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(VOLTDBX_BUILD_TESTS)
    enable_testing()
endif()

add_executable(voltdbx-bench
    src/bench/main.cpp
    src/bench/latency_runner.cpp
    src/storage/storage_engine.cpp
    src/storage/hash_table.cpp
    src/memory/buffer_pool.cpp
    src/memory/string_pool.cpp
)
target_include_directories(voltdbx-bench PRIVATE ${CMAKE_SOURCE_DIR}/include)

install(TARGETS voltdbx voltdbx-bench RUNTIME DESTINATION bin)
