#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 4096 // 4KB buffer size
#define MAX_USERNAME 32

// a struct to hold the username for each client
struct Client
{
    SOCKET socket;
    char username[MAX_USERNAME];
    int active; // keep it 1 if the slot is used, else let it be 0
};

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET server_sock, new_sock = {0}; // Initializing the client socket array to 0 for all clients
    struct Client clients[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address); // this is the length of the address that we have received

    fd_set readfds; // this is the set of file descriptors that we are going to use

    char buffer[BUFFER_SIZE] = {0}; // this is the buffer that we are going to use to store the messages that we are going to receive

    // initializing the winsock library
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0); // initializes the socket
    if (server_sock == INVALID_SOCKET)
    {
        perror("Couldn't create the damn socket");
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // bind to all available network interfaces
    address.sin_port = htons(PORT);       // host-to-network byte order conversion

    // binding the socket to the address
    if (bind(server_sock, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        perror("Couldn't bind the damn socket");
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }

    // now, we listen to the socket for client messages
    if (listen(server_sock, MAX_CLIENTS) == SOCKET_ERROR)
    {
        perror("Couldn't listen to the fucking socket man\n");
        closesocket(server_sock);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        FD_ZERO(&readfds);             // clear the file descriptor set
        FD_SET(server_sock, &readfds); // add the server socket to the set
        SOCKET max_sd = server_sock;   // initialize the maximum socket descriptor to the server socket

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            // socket descriptor
            SOCKET sd = clients[i].socket;
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        int activity = select(0, &readfds, NULL, NULL, NULL); // select() blocks until an activity is detected
        if (activity == SOCKET_ERROR)
        {
            printf("Select error.\n");
            continue;
        }

        // Checking for new connections
        if (FD_ISSET(server_sock, &readfds)) // if the server socket is in the set of file descriptors that we are monitoring for activity then we have a new connection to accept
                                             // from the client side and we need to add it to the client array of sockets that we have created earlier on in the code.
        {
            new_sock = accept(server_sock, (struct sockaddr *)&address, &addrlen); // accept the new connection
            if (new_sock == INVALID_SOCKET)
            {
                printf("Accept failed.\n");
                continue;
            }

            char usrName[MAX_USERNAME] = {0};
            recv(new_sock, usrName, MAX_USERNAME - 1, 0);
            printf("New client connected.\n");

            // Adding new socket to the client array
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i].active == 0)
                {
                    clients[i].socket = new_sock;
                    strncpy(clients[i].username, usrName, MAX_USERNAME - 1);
                    clients[i].active = 1;
                    printf("New Client :- %s connected \n", usrName);
                    break;
                }
            }
        }

        // Handling client messages
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            SOCKET sd = clients[i].socket;
            if (clients[i].active && FD_ISSET(sd, &readfds))
            {
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);
                if (valread <= 0)
                {
                    printf("Client disconnected.\n");
                    closesocket(sd);
                    clients[i].active = 0;
                    clients[i].socket = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("%s : %s\n", clients[i].username, buffer);
                }
            }
        }
    }

    // Cleanup (This will only execute if we break out of the loop)
    closesocket(server_sock);
    WSACleanup();
    return 0;
}
