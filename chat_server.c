#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET server_fd, new_socket, client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        printf("Socket failed.\n");
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        printf("Bind failed.\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen
    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR)
    {
        printf("Listen failed.\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        SOCKET max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET sd = client_sockets[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        int activity = select(0, &readfds, NULL, NULL, NULL);
        if (activity == SOCKET_ERROR)
        {
            printf("Select error.\n");
            continue;
        }

        if (FD_ISSET(server_fd, &readfds))
        {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket == INVALID_SOCKET)
            {
                printf("Accept failed.\n");
                continue;
            }

            printf("New client connected.\n");
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds))
            {
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);
                if (valread <= 0)
                {
                    printf("Client disconnected.\n");
                    closesocket(sd);
                    client_sockets[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("Client: %s\n", buffer);
                }
            }
        }
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
