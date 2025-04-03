#include "common.h"

typedef struct {
    int socket;
    char username[50];
    bool authenticated;
} ClientInfo;

ClientInfo clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_online_users() {
    ChatMessage msg;
    msg.type = LIST;
    memset(msg.content, 0, BUFFER_SIZE);
    
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != -1 && clients[i].authenticated) {
            strcat(msg.content, clients[i].username);
            strcat(msg.content, "\n");
        }
    }
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != -1 && clients[i].authenticated) {
            send(clients[i].socket, &msg, sizeof(msg), 0);
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);
    ChatMessage msg;
    
    // Receive authentication
    if (recv(client_socket, &msg, sizeof(msg), 0) <= 0 || msg.type != AUTH) {
        close(client_socket);
        return NULL;
    }
    
    // Check if username already exists
    pthread_mutex_lock(&clients_mutex);
    bool username_exists = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != -1 && clients[i].authenticated && 
            strcmp(clients[i].username, msg.username) == 0) {
            username_exists = true;
            break;
        }
    }
    
    if (username_exists) {
        msg.type = ERROR;
        strcpy(msg.content, "Username already taken");
        send(client_socket, &msg, sizeof(msg), 0);
        close(client_socket);
        pthread_mutex_unlock(&clients_mutex);
        return NULL;
    }
    
    // Add client to list
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == -1) {
            clients[i].socket = client_socket;
            strcpy(clients[i].username, msg.username);
            clients[i].authenticated = true;
            printf("Client %s connected\n", msg.username);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    // Send list of online users
    broadcast_online_users();
    
    // Handle messages
    while (1) {
        int recv_size = recv(client_socket, &msg, sizeof(msg), 0);
        if (recv_size <= 0) {
            break;
        }
        
        if (msg.type == DISCONNECT) {
            break;
        } else if (msg.type == MESSAGE) {
            pthread_mutex_lock(&clients_mutex);
            bool target_found = false;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].socket != -1 && clients[i].authenticated && 
                    strcmp(clients[i].username, msg.target) == 0) {
                    send(clients[i].socket, &msg, sizeof(msg), 0);
                    target_found = true;
                    break;
                }
            }
            
            if (!target_found) {
                msg.type = ERROR;
                strcpy(msg.content, "Target user not found");
                send(client_socket, &msg, sizeof(msg), 0);
            }
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    
    // Client disconnected
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == client_socket) {
            printf("Client %s disconnected\n", clients[i].username);
            clients[i].socket = -1;
            clients[i].authenticated = false;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    broadcast_online_users();
    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    
    // Initialize clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].socket = -1;
        clients[i].authenticated = false;
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server started on port %d. Waiting for connections...\n", SERVER_PORT);
    
    // Accept connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Check if server is full
        pthread_mutex_lock(&clients_mutex);
        bool server_full = true;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket == -1) {
                server_full = false;
                break;
            }
        }
        
        if (server_full) {
            ChatMessage msg;
            msg.type = ERROR;
            strcpy(msg.content, "Server is full. Try again later.");
            send(client_socket, &msg, sizeof(msg), 0);
            close(client_socket);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }
        pthread_mutex_unlock(&clients_mutex);
        
        // Create thread for client
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_socket;
        
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Thread creation failed");
            close(client_socket);
            free(new_sock);
            continue;
        }
        
        pthread_detach(thread_id);
    }
    
    close(server_socket);
    return 0;
}
