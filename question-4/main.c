#include "assembly_line.h"
#include <unistd.h>
#include <stdlib.h>

#define NUM_ITEMS 20

void* producer(void *arg) {
    AssemblyLine *line = (AssemblyLine*)arg;
    
    for (int i = 1; i <= NUM_ITEMS; i++) {
        assembly_line_add_item(line, i);
        sleep(2); // 2-second delay
    }
    
    return NULL;
}

void* consumer(void *arg) {
    AssemblyLine *line = (AssemblyLine*)arg;
    int item;
    int consumed = 0;
    
    while (consumed < NUM_ITEMS) {
        if (assembly_line_remove_item(line, &item)) {
            consumed++;
            sleep(3); // 3-second delay
        }
    }
    
    return NULL;
}

int main() {
    AssemblyLine line;
    assembly_line_init(&line);
    
    pthread_t producer_thread, consumer_thread;
    
    pthread_create(&producer_thread, NULL, producer, &line);
    pthread_create(&consumer_thread, NULL, consumer, &line);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    assembly_line_destroy(&line);
    
    printf("Production and consumption completed.\n");
    return 0;
}
