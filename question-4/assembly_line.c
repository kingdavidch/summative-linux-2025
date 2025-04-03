#include "assembly_line.h"
#include <stdio.h>

void assembly_line_init(AssemblyLine *line) {
    line->front = 0;
    line->rear = -1;
    line->count = 0;
    pthread_mutex_init(&line->mutex, NULL);
    pthread_cond_init(&line->not_full, NULL);
    pthread_cond_init(&line->not_empty, NULL);
}

void assembly_line_destroy(AssemblyLine *line) {
    pthread_mutex_destroy(&line->mutex);
    pthread_cond_destroy(&line->not_full);
    pthread_cond_destroy(&line->not_empty);
}

bool assembly_line_add_item(AssemblyLine *line, int item) {
    pthread_mutex_lock(&line->mutex);
    
    while (line->count == MAX_QUEUE_SIZE) {
        printf("Queue is full. Producer waiting...\n");
        pthread_cond_wait(&line->not_full, &line->mutex);
    }
    
    line->rear = (line->rear + 1) % MAX_QUEUE_SIZE;
    line->items[line->rear] = item;
    line->count++;
    
    printf("Producer added item %d. Queue size: %d\n", item, line->count);
    
    pthread_cond_signal(&line->not_empty);
    pthread_mutex_unlock(&line->mutex);
    return true;
}

bool assembly_line_remove_item(AssemblyLine *line, int *item) {
    pthread_mutex_lock(&line->mutex);
    
    while (line->count == 0) {
        printf("Queue is empty. Consumer waiting...\n");
        pthread_cond_wait(&line->not_empty, &line->mutex);
    }
    
    *item = line->items[line->front];
    line->front = (line->front + 1) % MAX_QUEUE_SIZE;
    line->count--;
    
    printf("Consumer took item %d. Queue size: %d\n", *item, line->count);
    
    pthread_cond_signal(&line->not_full);
    pthread_mutex_unlock(&line->mutex);
    return true;
}

int assembly_line_size(AssemblyLine *line) {
    pthread_mutex_lock(&line->mutex);
    int size = line->count;
    pthread_mutex_unlock(&line->mutex);
    return size;
}
