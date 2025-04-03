# Producer-Consumer Model Simulation in C

## Table of Contents
1. [Overview](#overview)
2. [Theoretical Background](#theoretical-background)
3. [Implementation Details](#implementation-details)
4. [Synchronization Mechanism](#synchronization-mechanism)
5. [Compilation and Execution](#compilation-and-execution)
6. [Expected Output](#expected-output)
7. [Analysis](#analysis)
8. [Conclusion](#conclusion)

## Overview

This project implements a classic producer-consumer problem simulation using POSIX threads in C. The scenario models a factory assembly line where:
- A producer thread manufactures items and adds them to a shared buffer (queue)
- A consumer thread removes items from the buffer for packaging
- The buffer has a maximum capacity of 10 items
- The producer operates with a 2-second delay between productions
- The consumer operates with a 3-second delay between consumptions

## Theoretical Background

The producer-consumer problem is a fundamental synchronization challenge in operating systems and concurrent programming where:

1. **Producers** generate data and place it in a buffer
2. **Consumers** remove data from the buffer
3. The system must ensure:
   - Producers don't add data to a full buffer
   - Consumers don't remove data from an empty buffer
   - Access to the buffer is thread-safe

This implementation solves these challenges using:
- Mutex locks for critical section protection
- Condition variables for efficient thread signaling
- A circular buffer queue data structure

## Implementation Details

### File Structure

1. **assembly_line.h**: Header file containing structure definitions and function declarations
2. **assembly_line.c**: Implementation of the assembly line queue operations
3. **main.c**: Contains the main program logic and thread functions
4. **Makefile**: Build automation file

### Key Components

1. **AssemblyLine Structure**:
   - Circular buffer array (`items[MAX_QUEUE_SIZE]`)
   - Front and rear pointers for queue management
   - Count of current items
   - Mutex for thread synchronization
   - Condition variables for buffer state signaling

2. **Core Functions**:
   - `assembly_line_init()`: Initializes the assembly line
   - `assembly_line_destroy()`: Cleans up resources
   - `assembly_line_add_item()`: Thread-safe item addition
   - `assembly_line_remove_item()`: Thread-safe item removal
   - `assembly_line_size()`: Thread-safe size query

3. **Thread Functions**:
   - `producer()`: Adds items with 2-second delays
   - `consumer()`: Removes items with 3-second delays

## Synchronization Mechanism

The implementation uses a robust synchronization approach:

1. **Mutex Lock**:
   - Protects all access to the shared queue
   - Ensures only one thread modifies the queue at any time

2. **Condition Variables**:
   - `not_full`: Signaled when space becomes available
   - `not_empty`: Signaled when items become available
   - Used with `pthread_cond_wait()` to avoid busy waiting

3. **Operation Flow**:
   - Producer waits on `not_full` when buffer is full
   - Consumer waits on `not_empty` when buffer is empty
   - Each operation signals the appropriate condition variable when complete

## Compilation and Execution

### Building the Program

```bash
make
```

This compiles the program using gcc with pthread support.

### Running the Simulation

```bash
./factory_simulation
```

### Cleaning Build Artifacts

```bash
make clean
```

## Expected Output

The program will output messages showing the production and consumption process:

```
Producer added item 1. Queue size: 1
Consumer took item 1. Queue size: 0
Producer added item 2. Queue size: 1
Producer added item 3. Queue size: 2
Consumer took item 2. Queue size: 1
...
Queue is full. Producer waiting...
Consumer took item 10. Queue size: 9
Producer added item 11. Queue size: 10
...
Production and consumption completed.
```

## Analysis

### Performance Characteristics

1. **Throughput**:
   - Producer rate: 0.5 items/second
   - Consumer rate: ~0.33 items/second
   - System will eventually be consumer-limited

2. **Buffer Behavior**:
   - Buffer will frequently reach capacity (10 items)
   - Producer will block until consumer makes space
   - Demonstrates backpressure mechanism

3. **Synchronization Effectiveness**:
   - No race conditions in queue access
   - No deadlocks possible
   - Efficient waiting via condition variables

### Correctness Verification

The implementation ensures:
1. **Safety**:
   - Mutual exclusion via mutex
   - Buffer bounds are never violated
2. **Liveness**:
   - Producers can always produce when space available
   - Consumers can always consume when items available
3. **Fairness**:
   - No thread starvation
   - FIFO ordering maintained

## Conclusion

This project successfully implements a thread-safe producer-consumer model that:
- Accurately simulates a factory assembly line
- Demonstrates proper synchronization techniques
- Handles all edge cases (full/empty buffer)
- Provides observable output of the synchronization process

The implementation serves as an excellent example of:
- POSIX thread programming
- Mutex and condition variable usage
- Circular buffer implementation
- Real-world concurrency problem solving

Further enhancements could include:
- Multiple producers/consumers
- Dynamic buffer sizing
- Performance metrics collection
- Graphical visualization of queue state
