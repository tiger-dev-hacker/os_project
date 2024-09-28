#include <stdio.h>
#include <pthread.h>

#define NUM_FACTORS 5 //Number of threads to be created
int arr[NUM_FACTORS] = {1,2,3,4,5}; //Array containing the numbers for which factorial will be calculated
int factorials[NUM_FACTORS];
int result = 0; //global variable to store the final results
pthread_mutex_t mutex;
typedef struct {
    int num; //number for which factorial is to be calculated;
    int index; //index of the array
    int factorial; //Factorial of the number;

}ThreadParams;

void *factorion(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    //print the thread id
    printf("Thread ID: %lu", pthread_self());
    //Calculate factorial and print
    int factorial; //Factorial of the number;
    params->factorial = 1;
    for(int i = 1; i <= params->num; i++)
    {
        params->factorial *= i;
    }

    factorials[params->index] = params->factorial; //store the factorial
    printf("Factorial for %d = %d", params->num, params->factorial);
    return NULL;
}

int main(){
    pthread_t threads[NUM_FACTORS];
    ThreadParams params[NUM_FACTORS];

    //Initialize the threads
    pthread_mutex_init(&mutex, NULL);
    //create threads
    for (int i = 0; i < NUM_FACTORS; i++)
    {
        params[i].num = arr[i];
        params[i].index = i;
        pthread_create(&threads[i], NULL ,factorion, &params[i]);
    }

    //Wait for all the threads to finish
    for (int i = 0; i < NUM_FACTORS; i++)
    {
        pthread_join(threads[i], NULL); 
    }
     
    int square; 
    //final the square of the factorial and add it to the global variable result [hint: it should be inside the loop]
    for(int i = 0; i < NUM_FACTORS; i++)
    {
        //Sum of squares
        square = factorials[i] * factorials[i];
        pthread_mutex_lock(&mutex);
        result += square; 
        pthread_mutex_unlock(&mutex);
    }
    //print the final result outside the loop
    printf("The summation of the squares of the factorials is: %d\n", result);

    // clean
    pthread_mutex_destroy(&mutex);
    return 0;
}
