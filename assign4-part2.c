#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MIN_SLEEP_MS 1
#define MAX_SLEEP_MS 500


pthread_mutex_t *chopsticks;
int num_philosophers;

void thinking(int threadIndex){
    int sleep_time = (rand() % MAX_SLEEP_MS) + MIN_SLEEP_MS;
    printf("Philosopher %d: thinking\n", threadIndex);
    usleep(sleep_time * 1000);
}

void pickUpChopsticks(int threadIndex) {
    int left = threadIndex;
    int right = (threadIndex + 1) % num_philosophers;

    pthread_mutex_lock(&chopsticks[left]);
    printf("Philosopher %d: trying to pick up left chopstick\n", threadIndex);
    

    usleep(rand() % 1000);

    pthread_mutex_lock(&chopsticks[right]);
    printf("Philosopher %d: trying to pick up right chopstick\n", threadIndex);
    

    
}

void eating(int threadIndex) {
    int sleep_time = (rand() % MAX_SLEEP_MS) + MIN_SLEEP_MS;
    printf("Philosopher %d: starts eating\n",threadIndex);
    usleep(sleep_time * 1000);
    printf("Philosopher %d: ends eating\n", threadIndex);
}

void putDownChopsticks(int threadIndex) {
    int left = threadIndex;
    int right = (threadIndex+1) % num_philosophers;

    pthread_mutex_unlock(&chopsticks[left]);
    printf("Philosopher %d: put down left chopstick\n", threadIndex);
    pthread_mutex_unlock(&chopsticks[right]);
    printf("Philosopher %d: put down right chopstick\n", threadIndex);

}

void* philosopher(void* index) {
    int threadIndex = *(int*) index;

    
         while(1) {
        thinking(threadIndex);
        pickUpChopsticks(threadIndex);
        eating(threadIndex);
        putDownChopsticks(threadIndex);
         }
    
}


int main() {
   

printf("Please enter number of philosophers:");
scanf("%d", &num_philosophers);

chopsticks = malloc(sizeof(pthread_mutex_t )* num_philosophers);
pthread_t threads[num_philosophers];
int philosopher_ids[num_philosophers];

for(int i = 0; i < num_philosophers; i++) {
    pthread_mutex_init(&chopsticks[i], NULL);
    philosopher_ids[i] = i;
}
for(int i = 0;  i < num_philosophers; i++) {
    philosopher_ids[i] = i;
    pthread_create(&threads[i],NULL, philosopher, &philosopher_ids[i]);
}

for(int i = 0; i < num_philosophers; i++) {
    pthread_join(threads[i], NULL);
}
for(int i = 0; i < num_philosophers; i++) {
    pthread_mutex_destroy(&chopsticks[i]);
}


free(chopsticks);

return 0;
}