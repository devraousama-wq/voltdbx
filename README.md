# VoltDBX

High-performance in-memory database server inspired by Redis, built with modern C++.

## Features

- TCP command server with concurrent client handling
- Key-value commands: GET, SET, DELETE, EXISTS, EXPIRE
- In-memory hash storage with memory pooling and arena allocation
- Disk snapshots and append-only recovery log
- TTL expiration with lazy delete and background sweep
- Pub/Sub channels with fan-out delivery
- kqueue/epoll accept loop with non-blocking I/O
- Built-in benchmarks and runtime INFO metrics

## Requirements

- CMake 3.16+
- C++17 compiler (GCC 9+, Clang 10+)
- POSIX sockets (Linux, macOS)

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Run server

```bash
./voltdbx --host 127.0.0.1 --port 6379 --data-dir ./data
```

Environment variables from `.env.example` override defaults when exported.

## Run benchmarks

```bash
./voltdbx-bench
```

## Protocol examples

```
SET session:42 active
GET session:42
EXISTS session:42
EXPIRE session:42 60
SUBSCRIBE alerts
PUBLISH alerts disk-full
INFO
```

## Project layout

```
include/voltdbx/   public headers
src/net/           sockets, accept loop, connection pool
src/storage/       hash table engine
src/persistence/   snapshots and AOF
src/pubsub/        channel broker
src/concurrency/   worker thread pool
src/bench/         latency harness
```

## License

MIT — see [LICENSE](LICENSE).
