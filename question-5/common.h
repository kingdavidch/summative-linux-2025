#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 4
#define BUFFER_SIZE 1024
#define SERVER_PORT 8888

typedef enum {
    AUTH,
    LIST,
    MESSAGE,
    ERROR,
    DISCONNECT
} MessageType;

typedef struct {
    MessageType type;
    char username[50];
    char target[50];
    char content[BUFFER_SIZE];
} ChatMessage;

#endif
