#ifndef ASSEMBLY_LINE_H
#define ASSEMBLY_LINE_H

#include <pthread.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 10

typedef struct {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} AssemblyLine;

void assembly_line_init(AssemblyLine *line);
void assembly_line_destroy(AssemblyLine *line);
bool assembly_line_add_item(AssemblyLine *line, int item);
bool assembly_line_remove_item(AssemblyLine *line, int *item);
int assembly_line_size(AssemblyLine *line);

#endif
