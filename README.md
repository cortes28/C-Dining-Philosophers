# C-Dining-Philosophers

Here we apply the synchronization dining philosophers problem in C using multi-threading and semaphores. Where the status of the philosophers are in a circular table of 5. There are 4 total statuses of {Thinking, Hungry, Eating} with the last one being the final state of {Full}. Each philosopher (5 total) is a thread alongside with 5 chopsticks that are semaphores that are shared amongst the philosophers (threads).
