#include "common.h"

int sock;
char username[50];
bool running = true;

void *receive_handler(void *arg) {
    ChatMessage msg;
    
    while (running) {
        int recv_size = recv(sock, &msg, sizeof(msg), 0);
        if (recv_size <= 0) {
            printf("Disconnected from server\n");
            running = false;
            exit(EXIT_FAILURE);
            break;
        }
        
        switch (msg.type) {
            case LIST:
                printf("\nOnline users:\n%s\n", msg.content);
                break;
            case MESSAGE:
                printf("\nMessage from %s: %s\n", msg.username, msg.content);
                break;
            case ERROR:
                printf("\nError: %s\n", msg.content);
                break;
            default:
                break;
        }
        
        printf("Enter username to chat with (or 'list' to see online users): ");
        fflush(stdout);
    }
    
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    pthread_t recv_thread;
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    // Authenticate
    ChatMessage msg;
    msg.type = AUTH;
    
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    strcpy(msg.username, username);
    
    if (send(sock, &msg, sizeof(msg), 0) < 0) {
        perror("Authentication failed");
        exit(EXIT_FAILURE);
    }
    
    // Wait for authentication response
    if (recv(sock, &msg, sizeof(msg), 0) <= 0) {
        perror("Authentication response failed");
        exit(EXIT_FAILURE);
    }
    
    if (msg.type == ERROR) {
        printf("Authentication error: %s\n", msg.content);
        exit(EXIT_FAILURE);
    }
    
    printf("Authenticated as %s\n", username);
    
    // Start receive thread
    if (pthread_create(&recv_thread, NULL, receive_handler, NULL) < 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Main loop for sending messages
    while (running) {
        printf("Enter username to chat with (or 'list' to see online users): ");
        char target[50];
        fgets(target, sizeof(target), stdin);
        target[strcspn(target, "\n")] = '\0';
        
        if (strcmp(target, "list") == 0) {
            msg.type = LIST;
            if (send(sock, &msg, sizeof(msg), 0) < 0) {
                perror("Send failed");
                break;
            }
            continue;
        }
        
        printf("Enter message (or 'exit' to quit): ");
        char content[BUFFER_SIZE];
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = '\0';
        
        if (strcmp(content, "exit") == 0) {
            msg.type = DISCONNECT;
            send(sock, &msg, sizeof(msg), 0);
            running = false;
            break;
        }
        
        msg.type = MESSAGE;
        strcpy(msg.target, target);
        strcpy(msg.content, content);
        
        if (send(sock, &msg, sizeof(msg), 0) < 0) {
            perror("Send failed");
            break;
        }
    }
    
    close(sock);
    pthread_join(recv_thread, NULL);
    return 0;
}
