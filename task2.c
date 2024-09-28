#include <stdio.h>
#include <pthread.h>

#define NUM_FACTORS 5 //Number of threads to be created
int arr[NUM_FACTORS] = {1,2,3,4,5}; //Array containing the numbers for which factorial will be calculated
int factorials[NUM_FACTORS];
int result = 0; //global variable to store the final results

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

    factorials[params->index] = factorial; //store the factorial
    printf("Factorial for %d = %d", params->num, params->factorial);
}

int main(){
    pthread_t threads[NUM_FACTORS];
    ThreadParams params[NUM_FACTORS];
    //final the square of the factorial and add it to the global variable result [hint: it should be inside the loop]
    //print the final result outside the loop
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

    //final the square of the factorial and add it to the global variable result [hint: it should be inside the loop]
    for(int i = 0; i < NUM_FACTORS; i++)
    {
        result += factorials[i] * factorials[i]; //Sum of squares
    }
    //print the final result outside the loop
    printf("The summation of the squares of the factorials is: %d\n", result);
    return 0;
}
