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
pthread_cond_t cond;
int waiting_customer = 0;

void* customer(void* num) {
    int id = *((int*)num);
    printf("Customer %d arrived\n", id);

    pthread_mutex_lock(&mutex);

    // Check if there are available chairs
    if (id == 1) { // Customer 1 gets to cut hair directly
        printf("Customer %d gets to cut hair directly\n", id);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_customer); // Signal the barber that a customer is waiting
    } else if (waiting_customer < n) {
        waiting_customer++;
        printf("Customer %d sits in waiting area (chair %d)\n", id, waiting_customer);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_customer); // Signal the barber that a customer is waiting
    } else {
        printf("Customer %d left, no chairs available\n", id);
        pthread_mutex_unlock(&mutex);
        free(num);
        return NULL; // Customer leaves without a haircut
    }

    // Wait for the barber to finish cutting hair
    sem_wait(&sem_barber);
    printf("Customer %d leaves after haircut\n", id);

    pthread_mutex_lock(&mutex);
    waiting_customer--;
    pthread_mutex_unlock(&mutex);

    free(num); // Free the dynamically allocated memory for the customer ID
    return NULL;
}

void* barber() {
    int customers_served = 0; // Counter for served customers
    while (1) {
        sem_wait(&sem_customer); // Wait for a customer to arrive

        pthread_mutex_lock(&mutex);
        int haircut_time = rand() % 5 + 1;
        printf("Barber starts cutting hair for %d seconds\n", haircut_time);
        pthread_mutex_unlock(&mutex);

        sleep(haircut_time);
        sem_post(&sem_barber); // Signal that the haircut is done

        pthread_mutex_lock(&mutex);
        customers_served++;
        if (customers_served == 4) {
            printf("Barber goes to sleep\n");
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t barber_thread;
    pthread_t customer_threads[m];

    sem_init(&sem_barber, 0, 0);
    sem_init(&sem_customer, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&barber_thread, NULL, barber, NULL);

    // Create customer 1
    int* id1 = malloc(sizeof(int));
    *id1 = 1;
    pthread_create(&customer_threads[0], NULL, customer, id1);

    // Wait for 1 second before creating customers 2, 3, and 4
    sleep(1);

    // Create customers 2, 3, and 4
    for (int i = 1; i <= 3; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&customer_threads[i], NULL, customer, id);
    }

    // Wait for 1 second before creating customer 5
    sleep(1);

    // Create customer 5
    int* id5 = malloc(sizeof(int));
    *id5 = 5;
    pthread_create(&customer_threads[4], NULL, customer, id5);

    for (int i = 0; i < m; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond); // Wake up the barber if he is sleeping
    pthread_mutex_unlock(&mutex);

    pthread_cancel(barber_thread);
    pthread_join(barber_thread, NULL);

    sem_destroy(&sem_barber);
    sem_destroy(&sem_customer);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}