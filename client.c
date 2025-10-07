#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// ANSI color codes for messages
#define COLOR_RESET "\x1b[0m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RED "\x1b[31m"

/**
 * Thread to continuously receive messages from server
 */
void *receive_messages(void *socket_fd)
{
    int sock = *(int *)socket_fd;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            printf(COLOR_RED "Disconnected from server.\n" COLOR_RESET);
            break;
        }
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }

    exit(0); // terminate client if disconnected
    return NULL;
}

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char message[BUFFER_SIZE];

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Start receive thread
    pthread_t tid;
    pthread_create(&tid, NULL, receive_messages, &sock);
    pthread_detach(tid);

    // Handle sending messages
    // Handle sending messages
    while (1)
    {
        if (fgets(message, sizeof(message), stdin) == NULL)
            break;

        // Remove trailing newline from input
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n')
        {
            message[len - 1] = '\0';
        }

        // Skip empty messages
        if (strlen(message) == 0)
            continue;

        // Send the message to the server
        send(sock, message, strlen(message), 0);

        // If user types /quit, exit
        if (strncmp(message, "/quit", 5) == 0)
            break;
    }

    close(sock);
    printf(COLOR_RED "You have left the chat.\n" COLOR_RESET);
    return 0;
}
