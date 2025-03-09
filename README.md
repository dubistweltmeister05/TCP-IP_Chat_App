# Windows TCP/IP Chat Application

A robust command-line chat application implemented in C using Windows Sockets 2 (Winsock2) API. This application demonstrates basic client-server network programming concepts through a TCP/IP-based chat system that supports multiple concurrent client connections.

## 🌟 Features

### Server Capabilities

- Concurrent handling of up to 10 client connections
- Non-blocking I/O using select() for efficient message handling
- Real-time message broadcasting
- Automatic client disconnect detection
- Dynamic client socket management
- Detailed connection status logging

### Client Features

- Simple command-line interface for message input
- Persistent connection to server
- Real-time message transmission
- Local echo of sent messages

## 🔧 Technical Architecture

### Server Architecture

- **Network Protocol**: TCP/IP
- **Socket Type**: SOCK_STREAM (connection-oriented)
- **Port**: 8080 (configurable via PORT macro)
- **Buffer Size**: 4KB (4096 bytes)
- **Client Management**: Array-based client socket tracking
- **I/O Multiplexing**: fd_set based select() implementation
- **Address Binding**: INADDR_ANY (accepts connections on all network interfaces)

### Client Architecture

- **Network Protocol**: TCP/IP
- **Socket Type**: SOCK_STREAM
- **Buffer Size**: 1KB (1024 bytes)
- **Connection Type**: Persistent TCP connection
- **Target Address**: localhost (127.0.0.1)

## 📋 Prerequisites

### System Requirements

- Windows operating system (Windows 7 or later)
- C compiler with Windows SDK support
- Minimum 64MB RAM
- Network connectivity for client-server communication

### Development Requirements

- C compiler (GCC, MSVC, etc.)
- Windows SDK or MinGW
- Winsock2 library (ws2_32.lib)
- Basic understanding of network programming concepts

## 🚀 Building the Application

### Using GCC

```bash
# Compile the server with debugging symbols
gcc -g chat_server.c -o chat_server -lws2_32 -Wall

# Compile the client with debugging symbols
gcc -g chat_client.c -o chat_client -lws2_32 -Wall
```

### Using MSVC

```bash
# Compile the server
cl chat_server.c /link ws2_32.lib

# Compile the client
cl chat_client.c /link ws2_32.lib
```

## 📖 Usage Guide

### Server Deployment

1. Open Command Prompt with administrator privileges
2. Navigate to the executable directory
3. Execute the server:

```bash
chat_server.exe
```

4. Server initialization sequence:
   - Winsock initialization
   - Socket creation
   - Address binding
   - Listen state activation
   - Ready for client connections

### Client Operation

1. Open a new Command Prompt window
2. Navigate to the executable directory
3. Launch the client:

```bash
chat_client.exe
```

4. Client connection sequence:
   - Winsock initialization
   - Socket creation
   - Server connection establishment
   - Message input prompt
5. Type messages and press Enter to send

## 🔍 Implementation Details

### Server Implementation

- **Client Management**: Uses a fixed array of MAX_CLIENTS (10) socket descriptors
- **Message Handling**: Asynchronous message processing using select()
- **Buffer Management**: Zero-initialized 4KB buffer for message handling
- **Error Handling**: Comprehensive error checking for socket operations
- **Resource Management**: Proper socket cleanup on client disconnect

### Client Implementation

- **Connection Management**: Single persistent connection to server
- **Input Handling**: Blocking fgets() for user input
- **Buffer Management**: 1KB message buffer
- **Error Handling**: Basic error checking for connection and send operations

## ⚠️ Limitations and Considerations

### Current Limitations

- Fixed maximum client limit (10 concurrent connections)
- No message persistence
- No user authentication
- No encryption or security features
- Single-threaded implementation
- No private messaging capability
- Fixed buffer sizes
- No connection recovery mechanism

### Performance Considerations

- Server performance may degrade with increasing client connections
- Large messages may require multiple send/receive operations
- No built-in flow control mechanisms
- Memory usage scales linearly with client connections

## 🛠️ Future Improvements

Potential enhancements that could be implemented:

1. User authentication system
2. Message encryption using SSL/TLS
3. Database integration for message persistence
4. Private messaging functionality
5. File transfer capabilities
6. Dynamic buffer sizing
7. Connection recovery mechanism
8. Multi-threading for improved performance
9. GUI implementation
10. Configuration file support

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add appropriate tests
5. Submit a pull request

Please ensure your code follows the existing style and includes appropriate comments.

## 📞 Support

For issues, questions, or contributions, please:

1. Create an issue in the repository
2. Document the problem or suggestion clearly
3. Include relevant code snippets or error messages
4. Specify your environment details

## 🔄 Version History

- v1.0.0 - Initial release
  - Basic client-server functionality
  - Multiple client support
  - Real-time message handling
