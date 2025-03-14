#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 4096

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Fix: Use inet_addr() instead of inet_pton()
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Connection failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to server. Type messages:\n");

    char username[32];
    printf("WHat's your name mate?\n");
    fgets(username, sizeof(username), stdin); // read from the user via stdin stream, store in username
    username[strcspn(username, "\n")] = 0;

    send(sock, username, strlen(username), 0);

    printf("COnnected to the server as &s. Send in the messages\n", username);
    while (1)
    {
        char message[BUFFER_SIZE];
        fgets(message, BUFFER_SIZE, stdin);

        // Format: "username: message"
        char formatted_msg[BUFFER_SIZE + 32];
        snprintf(formatted_msg, sizeof(formatted_msg), "%s: %s", username, message);
        send(sock, formatted_msg, strlen(formatted_msg), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
