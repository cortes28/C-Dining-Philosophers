#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
/*
terminal call and run (ubuntu WSL using gcc)\

gcc dining_philosophers_pthr.c  -o dining_philosophers_pthr -lpthread
./dining_philosophers_pthr

* may need lpthread library to run*
*/ 



// Bryan Cortes my own philosopher alternating odd/even 
// This is my version with my code with odd picking up right and left
// and even picking up left and then right

// in function void status_(void) -> has a usleep function call. The value can be changed depending on the speed desired or removed entirely.


// our 5 chopsticks for the philosophers
sem_t chopstick[5];
// allowing only one to eat...then drop chopsticks
sem_t mutex;
// allowing up to 4 philosophers sit down
sem_t table;

// for displaying information regarding the philosophers
sem_t call;

// status of the philopopher
enum status {
    THINKING = 0,
    HUNGRY = 1,
    EATING = 2,
    FULL = 3   
};

// the number of times each philosopher will eat
#define HUNGERBAR_1 5
#define HUNGERBAR_2 HUNGERBAR_1
#define HUNGERBAR_3 HUNGERBAR_2
#define HUNGERBAR_4 HUNGERBAR_3
#define HUNGERBAR_5 HUNGERBAR_4

// will be used to determine the state of each philosopher
int state[5];

void status_(void){
    sem_wait(&call);
    printf("\n============================================\n");
    for (int i = 0; i < 5;i++){
        switch(state[i]){
            case THINKING:
                printf("Philosopher %d is THINKING...\n", (i+1));
                break;
            case HUNGRY:
                printf("Philosopher %d is HUNGRY...\n", (i+1));
                break;
            case EATING:
                printf("Philosopher %d is EATING...\n", (i+1));
                break;
            case FULL:
                printf("Philosopher %d is FULL...\n", (i+1));
                break;
            default:
                printf("Who is this?\n");
        }

    }
    printf("\n============================================\n");
    // Value can be changed for a faster runtime or slower runtime depending on the speed of the philophers eating,
    // can be removed entirely
    usleep(100000);     
    sem_post(&call);
}

// our 5 philosophers
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
// Odd (1, 3, 5)philosphers will pick up left chopsticks first which would be 
// picking up left first then right 

// Even (2, 4) philosophers would pick up right then left

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//



void * philosopher_odd1(void * param){
int hunger_bar = HUNGERBAR_5;
    int pnum = 0;
  
    while(hunger_bar-- > 0){
        status_();
        sem_wait(&table);                                    // sit on table while there are no more than 4      
        state[pnum] = HUNGRY;
        status_();
        // to pick up chopsticks right then left 
        sem_wait(&chopstick[(pnum + 1) % 5]);
        sem_wait(&chopstick[pnum]);
        // Philosopher is now eating
        sem_wait(&mutex);
        state[pnum] = EATING;
        status_();
        usleep(100);

        // releasing chopsticks and changing status to think 
        sem_post(&chopstick[(pnum + 1) % 5]);
        sem_post(&chopstick[pnum]);
        state[pnum] = THINKING;
        status_();

        sem_post(&mutex);
        // get out of the table to let another philosopher to sit down and eat
        sem_post(&table);       
                                  
        usleep(100);
    }
    state[pnum] = FULL;
    pthread_exit(0);

    return NULL;
}

void * philosopher_even2(void * param){
   int hunger_bar = HUNGERBAR_1;
    int pnum = 1;

    while(hunger_bar-- > 0){
        status_();
        sem_wait(&table);                                   // sit on table while there are no more than 4
        state[pnum] = HUNGRY;
        status_();
        
        // to pick up chopsticks left then right 
        sem_wait(&chopstick[pnum]);
        sem_wait(&chopstick[(pnum + 1) % 5]);               // % 5 if chopstick '5' is picked -> pick chopstick 0
        // Philosopher is now eating
        sem_wait(&mutex);                                   // lock mutex to eat using sem_wait 'only 1 can eat'
        state[pnum] = EATING;
        status_();
        usleep(100);

        // releasing chopsticks and changing status to think
        sem_post(&chopstick[pnum]);
        sem_post(&chopstick[(pnum + 1) % 5]);
        state[pnum] = THINKING;
        status_();

        sem_post(&mutex);
        // get out of the table to let another philosopher to sit down and eat

        sem_post(&table);                                   // get off the table
        usleep(100);
    }

    state[pnum] = FULL;
    pthread_exit(0);

    return NULL;
}

void * philosopher_odd3(void * param){
    int hunger_bar = HUNGERBAR_5;
    int pnum = 2;
  
    while(hunger_bar-- > 0){
        status_();
        sem_wait(&table);                                    // sit on table while there are no more than 4      
        state[pnum] = HUNGRY;
        status_();
        // to pick up chopsticks right then left 
        sem_wait(&chopstick[(pnum + 1) % 5]);
        sem_wait(&chopstick[pnum]);
        // Philosopher is now eating
        sem_wait(&mutex);
        state[pnum] = EATING;
        status_();
        usleep(100);

        // releasing chopsticks and changing status to think
        sem_post(&chopstick[(pnum + 1) % 5]);
        sem_post(&chopstick[pnum]);
        state[pnum] = THINKING;
        status_();

        sem_post(&mutex);
        // get out of the table to let another philosopher to sit down and eat
        sem_post(&table);       
                                  
        usleep(100);
    }

    state[pnum] = FULL;
    pthread_exit(0);

    return NULL;
}

void * philosopher_even4(void * param){
     int hunger_bar = HUNGERBAR_1;
    int pnum = 3;

    while(hunger_bar-- > 0){
        status_();
        sem_wait(&table);                                   // sit on table while there are no more than 4
        state[pnum] = HUNGRY;
        status_();
        
        // to pick up chopsticks left then right 
        sem_wait(&chopstick[pnum]);
        sem_wait(&chopstick[(pnum + 1) % 5]);
        // Philosopher is now eating
        sem_wait(&mutex);
        state[pnum] = EATING;
        status_();
        usleep(100);

        // releasing chopsticks and changing status to think
        sem_post(&chopstick[pnum]);
        sem_post(&chopstick[(pnum + 1) % 5]);
        state[pnum] = THINKING;
        status_();

        sem_post(&mutex);
        // get out of the table to let another philosopher to sit down and eat

        sem_post(&table);                                   // get off the table
        usleep(100);
    }

    state[pnum] = FULL;
    pthread_exit(0);

    return NULL;
}

void * philosopher_odd5(void * param){
    int hunger_bar = HUNGERBAR_5;
    int pnum = 4;
  
    while(hunger_bar-- > 0){
        status_();
        sem_wait(&table);                                    // sit on table while there are no more than 4      
        state[pnum] = HUNGRY;
        status_();
        // to pick up chopsticks right then left 
        sem_wait(&chopstick[(pnum + 1) % 5]);
        sem_wait(&chopstick[pnum]);
        // Philosopher is now eating
        sem_wait(&mutex);
        state[pnum] = EATING;
        status_();
        usleep(100);

        // releasing chopsticks and changing status to think
        sem_post(&chopstick[(pnum + 1) % 5]);
        sem_post(&chopstick[pnum]);
        state[pnum] = THINKING;
        status_();

        sem_post(&mutex);
        // get out of the table to let another philosopher to sit down and eat
        sem_post(&table);       
                                  
        usleep(100);
    }

    state[pnum] = FULL;
    pthread_exit(0);

    return NULL;
}

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//

int main(int argc, char * argv[]){
    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_attr_t attr;

    // assigned the appropiate semaphore, 0 as it will be a shared semaphore, and value for the semaphore
    sem_init(&mutex, 0, 1);
    sem_init(&table, 0, 4);
    sem_init(&call, 0, 1);

    // initialize each chopstick with the value of 0 as its shared and 1 as it can only be used ONCE per call. 
    for(int i = 0; i < 5; i++){
        sem_init(&chopstick[i], 0, 1);
    }

    // initialize to thinking to all
    for(int i = 0; i < 5; i++){
        state[i] = THINKING;
    }

    printf("\n\nOur philosophers will now dine in!\n\n");

    pthread_attr_init(&attr);

    // create each thread for each philosopher
    pthread_create(&tid1, &attr, philosopher_odd1,  (void*)0);
    pthread_create(&tid2, &attr, philosopher_even2, (void*)1);
    pthread_create(&tid3, &attr, philosopher_odd3,  (void*)2);
    pthread_create(&tid4, &attr, philosopher_even4, (void*)3);
    pthread_create(&tid5, &attr, philosopher_odd5,  (void*)4);

    // wait for all to join
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

    status_();
    printf("\nPhilosophers are now done eating for today!\n");

    // get rid of the semaphores
    for(int i = 0; i < 5; i++){
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

