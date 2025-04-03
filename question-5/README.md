# Linux Programming Chat System

## Overview

This project implements a fully functional chat system with a central server and multiple clients (up to 4) as part of a Linux Programming course. The system demonstrates core concepts of socket programming, multi-threading, and inter-process communication in a Linux environment.

## Key Features

- **Centralized Server Architecture**: Manages all client connections and message routing
- **Client Authentication**: Users must login with unique usernames
- **Online User Tracking**: Real-time updates of connected clients
- **Bidirectional Communication**: Clients can send and receive messages simultaneously
- **Thread-Safe Implementation**: Uses mutex locks for shared data protection
- **Instant Messaging**: Real-time message delivery between clients

## System Architecture

The chat system follows a client-server model with the following components:

1. **Chat Server** (`chat_server.c`):
   - Manages all client connections
   - Handles authentication and username validation
   - Maintains list of online users
   - Routes messages between clients
   - Enforces system limits (max 4 clients)

2. **Chat Client** (`chat_client.c`):
   - Establishes connection to server
   - Handles user authentication
   - Provides interface for sending/receiving messages
   - Displays online users
   - Runs separate thread for receiving messages

3. **Common Definitions** (`common.h`):
   - Shared data structures
   - Message type definitions
   - Network configuration constants

## Implementation Details

### Server-Side Implementation

1. **Socket Creation and Binding**:
   - Creates TCP socket on port 8888
   - Binds to all available network interfaces
   - Listens for incoming connections

2. **Client Handling**:
   - Accepts new connections in main thread
   - Spawns separate thread for each client
   - Uses mutex locks to protect shared client list

3. **Message Processing**:
   - Validates authentication messages
   - Maintains updated list of online users
   - Routes messages to intended recipients
   - Handles client disconnections gracefully

### Client-Side Implementation

1. **Connection Establishment**:
   - Connects to server at 127.0.0.1:8888
   - Sends authentication with username
   - Verifies successful login

2. **User Interface**:
   - Provides simple text-based interface
   - Shows list of online users
   - Allows selecting recipients and sending messages

3. **Multi-threading**:
   - Main thread handles user input and message sending
   - Separate thread handles incoming messages
   - Uses running flag for clean shutdown

## Technical Components

1. **Socket Programming**:
   - Uses BSD sockets API
   - TCP protocol for reliable communication
   - IPv4 addressing

2. **Threading**:
   - POSIX threads (pthreads)
   - Mutex locks for thread synchronization
   - Detached threads for client handling

3. **Message Protocol**:
   - Custom message format defined in `common.h`
   - Type field indicates message purpose (auth, list, chat, etc.)
   - Fixed-size structure for simplicity

## Build Instructions

1. Compile the system using the provided Makefile:
   ```bash
   make
   ```

2. This will create two executables:
   - `chat_server` - The central server program
   - `chat_client` - The client program

## Running the System

1. Start the server in one terminal:
   ```bash
   ./chat_server
   ```

2. Start up to 4 clients in separate terminals:
   ```bash
   ./chat_client
   ```

3. Follow the on-screen instructions to:
   - Enter a username
   - View online users
   - Select a recipient
   - Send messages

## Testing Methodology

The system was tested with the following scenarios:

1. **Single Client**:
   - Verify successful connection
   - Check authentication
   - Confirm empty user list

2. **Multiple Clients**:
   - Connect 4 clients simultaneously
   - Verify all appear in each other's user lists
   - Test message exchange between all pairs

3. **Edge Cases**:
   - Duplicate usernames (should be rejected)
   - Fifth connection attempt (should be rejected)
   - Disconnected clients (should be removed from lists)
   - Long messages (should be truncated to buffer size)

## Known Limitations

1. **Scalability**:
   - Hard-coded limit of 4 clients
   - No load balancing for large numbers of clients

2. **Security**:
   - No encryption of network traffic
   - Simple username-based authentication

3. **Features**:
   - No message history persistence
   - No group chat functionality
   - Basic text interface without rich formatting

## Future Enhancements

1. **Improved Security**:
   - Add password authentication
   - Implement TLS encryption

2. **Additional Features**:
   - Group chats and channels
   - Message history and logging
   - File transfer capability

3. **Scalability Improvements**:
   - Dynamic client limit
   - Multiple server instances with load balancing

## Conclusion

This project successfully demonstrates the core principles of network programming in Linux, including socket communication, multi-threading, and client-server architecture. The implementation meets all specified requirements while providing a foundation for future enhancements.

## License

This project is for academic purposes as part of a Linux Programming course. All code is provided as-is for educational use.
