# philosophers
sources:
https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

https://medium.com/@denaelgammal/dining-philosophers-problem-42-project-guide-mandatory-part-a20fb8dc530e

https://www.youtube.com/watch?v=LOfGJcVnvAk

https://fr.wikipedia.org/wiki/D%C3%AEner_des_philosophes

https://www.youtube.com/watch?v=ldJ8WGZVXZk

https://franckh.developpez.com/tutoriels/posix/pthreads/

argv 1 : number_of_philosophers
argv 2 : time_to_die
argv 3 : time_to_eat
argv 4 : time_to_sleep
argv 5 : [number_of_times_each_philosopher_must_eat]

# Philosophers Project

A C implementation of the classic Dining Philosophers problem, demonstrating process synchronization and deadlock prevention.

## Overview

This project simulates philosophers sitting at a round table, alternating between eating, thinking, and sleeping. Each philosopher needs two forks to eat, and must avoid deadlocks while sharing resources with their neighbors.

## Requirements

- C compiler (gcc recommended)
- POSIX threads library (pthread)
- Unix-like operating system

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parameters

- `number_of_philosophers`: Number of philosophers at the table
- `time_to_die`: Time in milliseconds until a philosopher dies from starvation
- `time_to_eat`: Time in milliseconds it takes to eat
- `time_to_sleep`: Time in milliseconds to sleep
- `number_of_times_each_philosopher_must_eat`: (Optional) Program stops when all philosophers eat this many times

## Implementation Details

- Uses mutex locks for fork management
- Implements death monitoring system
- Handles resource sharing and deadlock prevention
- Includes precise timing mechanisms

## Key Features

- Thread-safe operations
- Real-time status monitoring
- Configurable simulation parameters
- Death detection system

