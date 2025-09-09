# Readme for Assignment 5: Synchronization

## Overview
This program solves the **Bounded Buffer Problem** using POSIX threads, semaphores, and mutex locks. It demonstrates thread synchronization by implementing producers and consumers working concurrently on a shared circular buffer.

## Files
- `assignment_5.c`: The main C program implementing the bounded buffer solution.

## How to Compile
To compile the program, use the following command:
```bash
gcc -o assignment_5 assignment_5.c -lpthread
```
This command compiles the program with the necessary pthread library.

## How to Run
The program requires three command-line arguments:
1. **`<sleep_time>`**: The number of seconds the main thread sleeps before terminating the program.
2. **`<num_producers>`**: The number of producer threads to create.
3. **`<num_consumers>`**: The number of consumer threads to create.

### Usage
Run the program with:
```bash
./assignment_5 <sleep_time> <num_producers> <num_consumers>
```

### Example
To run the program with:
- Sleep time: 10 seconds
- 2 producer threads
- 3 consumer threads

Use:
```bash
./assignment_5 10 2 3
```

## Program Output
The program will output the actions of producers and consumers, such as:
- Producers inserting items into the buffer
- Consumers removing items from the buffer

### Example Output
```
Producer 0 produced item 45
Producer inserted item 45 into buffer[0]
Consumer 0 consumed item 45
Consumer removed item 45 from buffer[0]
...
Main thread terminating
```

### Notes
- The behavior of the program is randomized, so the exact sequence of operations may vary.
- Each action respects the synchronization constraints to ensure correctness.

## Important Considerations
1. **Synchronization**:
   - The program uses semaphores (`empty` and `full`) to track buffer slots and a mutex lock (`mutex`) to ensure mutual exclusion during buffer access.

2. **Circular Buffer**:
   - The buffer is implemented as a fixed-size array of size 5, with indices wrapping around using modulo arithmetic.
