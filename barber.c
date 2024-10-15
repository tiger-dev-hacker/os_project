#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int n = 3; // waiting chairs 
int m = 5; // customers 

sem_t sem_barber;
sem_t sem_customer;
pthread_mutex_t mutex;
int current_customer_id = 0;
int waiting_customer = 0;
int barber_awake = 0;

void* customer(void* num) {
    int id = *((int*)num);
    printf("Customer %d arrived\n", id);
    pthread_mutex_lock(&mutex);


    if(waiting_customer == 0 && !barber_awake) {
        printf("Customer %d wakes up barber\n", id);
        barber_awake = 1;
    }
        pthread_mutex_unlock(&mutex);


    if(waiting_customer < n) {
         waiting_customer++;
        printf("Customer %d sits in waiting area (chair %d)\n", id, waiting_customer);
    
       
    } else {
        printf("Customer %d left, no chairs available", id);
    }


        sem_post(&sem_customer);
        sem_wait(&sem_barber);
        printf("Customer %d leaves after haircut\n", id); 
}

 
    

void* barber() {
    while(1) {
        sem_wait(&sem_customer);

        pthread_mutex_lock(&mutex);
        current_customer_id++;
        waiting_customer--;

        int haircut_time = rand() % 5 + 1;
        printf("Barber starts cutting hair of customer %d for %d secomds\n", current_customer_id, haircut_time);
        pthread_mutex_unlock(&mutex);

        sleep(haircut_time);
        sem_post(&sem_barber);

        pthread_mutex_lock(&mutex);
        if(waiting_customer == 0)
        {
            printf("Barber goes to sleep\n");
            barber_awake = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[m];

    sem_init(&sem_barber,0,0);
    sem_init(&sem_customer,0,0);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&barber_thread, NULL, barber, NULL);

    for(int i = 0; i < m; i++)
    {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customer_threads[i],NULL,customer, id);
        sleep(1);
    }

    for(int i = 0; i < m; i++)
    {
        pthread_join(customer_threads[i], NULL);
    }

    pthread_cancel(barber_thread);
    pthread_join(barber_thread, NULL);

    sem_destroy(&sem_barber);
    sem_destroy(&sem_customer);
    pthread_mutex_destroy(&mutex);

    return 0;
}
