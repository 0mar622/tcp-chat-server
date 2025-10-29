# Multi-Client TCP Chat Server

A C++ program that implements a concurrent chat server using TCP sockets. The server handles multiple clients simultaneously through the `select()` system call, allowing real-time communication between connected clients (e.g., via `nc` / netcat). External clients can connect to the server from different terminals or machines on the same network.

## Current Features
- Accepts multiple simultaneous client connections
- Broadcasts messages to all connected clients in real time
- Uses `select()` for I/O multiplexing instead of multithreading
- Supports external clients via `nc` or telnet

## Planned Features
- Build dedicated C++ client program for user interaction  
- Add message history logging with file I/O  
- Implement user commands (e.g., private messages, username registration)

## Tech Stack
C++ | TCP Sockets | select() | Linux | Networking

## Status
In Progress – Server fully functional; client program in development.

## What I’m Learning
Deep understanding of socket programming, concurrent I/O handling with `select()`, and scalable connection management.
