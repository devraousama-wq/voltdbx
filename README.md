# VoltDBX

High-performance in-memory database server inspired by Redis, built with modern C++.

## Features

- TCP command server with concurrent client handling
- Key-value commands: GET, SET, DELETE, EXISTS
- In-memory hash storage with memory pooling
- Disk snapshots and append-only recovery log
- TTL expiration with lazy and active cleanup
- Pub/Sub channels with fan-out delivery
- Built-in benchmarks and runtime monitoring

## Requirements

- CMake 3.16+
- C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+)
- POSIX sockets (Linux, macOS)

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Run

```bash
./voltdbx --host 127.0.0.1 --port 6379
```

Copy `.env.example` to `.env` to override defaults.

## Protocol

Line-oriented text protocol over TCP. Example:

```
SET mykey hello
GET mykey
```

## License

MIT — see [LICENSE](LICENSE).
