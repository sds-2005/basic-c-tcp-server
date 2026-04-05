# C TCP Client-Server Chat

A fundamental implementation of a synchronous, single-threaded client-server chat application using POSIX sockets in C. 

This project was built to understand the core networking pipeline: `socket()`, `bind()`, `listen()`, `accept()`, and standard network byte order conversions.

## How to Run

**1. Compile the code:**
```bash
gcc server.c -o server
gcc client.c -o client
```

**2. Start the Server:**
Provide a port number as an argument.
```bash
./server 8080
```

**3. Start the Client:**
Connect using the server's IP (or localhost) and the same port number.
```bash
./client 127.0.0.1 8080
```