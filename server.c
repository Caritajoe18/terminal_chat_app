#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define NAME_LEN 32

// ANSI color codes for terminal messages
#define COLOR_RESET "\x1b[0m"
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE "\x1b[34m"

// Structure to store client info
typedef struct
{
    int socket;
    char name[NAME_LEN];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Broadcast a message to all clients except the sender
 */
void broadcast(char *message, int sender_socket)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket != sender_socket)
        {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/**
 * Send a private message to a specific client
 */
void send_private(char *message, char *target_name, int sender_socket)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (strcmp(clients[i].name, target_name) == 0)
        {
            send(clients[i].socket, message, strlen(message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

/**
 * Save message to chat log
 */
void log_message(char *message)
{
    FILE *fp = fopen("chat_history.txt", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s\n", message); // add newline explicitly
        fclose(fp);
    }
}

/**
 * Handle client commands such as /list or /quit
 */
int handle_command(char *command, int client_socket)
{
    if (strncmp(command, "/list", 5) == 0)
    {
        char user_list[BUFFER_SIZE] = "Connected users:\n";
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < client_count; i++)
        {
            strcat(user_list, clients[i].name);
            strcat(user_list, "\n");
        }
        pthread_mutex_unlock(&clients_mutex);
        send(client_socket, user_list, strlen(user_list), 0);
        return 1;
    }
    else if (strncmp(command, "/quit", 5) == 0)
    {
        return -1; // signal to disconnect
    }
    return 0; // not a command
}

/**
 * Handle individual client communication
 */
void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg); // free the malloc'd socket pointer

    char buffer[BUFFER_SIZE];
    char name[NAME_LEN];

    // Ask client for nickname
    send(client_socket, "Enter your nickname: ", strlen("Enter your nickname: "), 0);

    // Reserve one byte for the terminating NUL
    int bytes = recv(client_socket, name, NAME_LEN - 1, 0);
    if (bytes <= 0)
    {
        close(client_socket);
        return NULL;
    }

    // Null-terminate properly
    name[bytes] = '\0';

    size_t nlen = strlen(name);
    if (nlen > 0 && (name[nlen - 1] == '\n' || name[nlen - 1] == '\r'))
    {
        name[nlen - 1] = '\0';
    }

    // Add client to array
    pthread_mutex_lock(&clients_mutex);
    clients[client_count].socket = client_socket;
    strncpy(clients[client_count].name, name, NAME_LEN);
    client_count++;
    pthread_mutex_unlock(&clients_mutex);

    // Notify others
    char join_msg[BUFFER_SIZE];
    snprintf(join_msg, sizeof(join_msg), COLOR_GREEN "%s has joined the chat!" COLOR_RESET, name);
    broadcast(join_msg, client_socket);
    log_message(join_msg);
    printf("%s joined.\n", name);

    // Receive messages
    while (1)
    {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';

        // Check if message is a command
        if (buffer[0] == '/')
        {
            int result = handle_command(buffer, client_socket);
            if (result == -1)
                break; // /quit
            continue;
        }

        // Check if message is private (@username message)
        if (buffer[0] == '@')
        {
            char target[NAME_LEN];
            sscanf(buffer, "@%s", target);
            char *msg_body = strchr(buffer, ' ');
            if (msg_body)
            {
                char pm[BUFFER_SIZE];
                snprintf(pm, sizeof(pm), COLOR_BLUE "[PM from %s]: %s" COLOR_RESET, name, msg_body + 1);
                send_private(pm, target, client_socket);
                log_message(pm);
            }
        }
        else
        {
            // Broadcast normal message
            char msg[BUFFER_SIZE];
            snprintf(msg, sizeof(msg), COLOR_BLUE "%s: %s" COLOR_RESET, name, buffer);
            broadcast(msg, client_socket);
            log_message(msg);
        }
    }

    // Remove client
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket == client_socket)
        {
            clients[i] = clients[client_count - 1]; // replace with last client
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(client_socket);
    snprintf(buffer, sizeof(buffer), COLOR_RED "%s has left the chat." COLOR_RESET, name);
    broadcast(buffer, client_socket);
    log_message(buffer);
    printf("%s disconnected.\n", name);
    return NULL;
}

int main()
{
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //any computer can connect
    server_addr.sin_port = htons(PORT); 

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (new_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        /* allocate an int on heap so each thread gets its own copy */
        int *pclient = malloc(sizeof(int));
        if (!pclient)
        {
            perror("malloc");
            close(new_socket);
            continue;
        }
        *pclient = new_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}
