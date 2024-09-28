#include <stdio.h>
#include <pthread.h>

#define NUM_FACTORS 5 //Number of threads to be created
int arr[NUM_FACTORS] = {1,2,3,4,5}; //Array containing the numbers for which factorial will be calculated
int result = 0; //global variable to store the final results

typedef struct {
    int num; //number for which factorial is to be calculated;
    int factorial; //Factorial of the number;
}ThreadParams;

void *factorion(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    //print the thread id;
    //Calculate factorial and print
}

int main(){
    //final the square of the factorial and add it to the global variable result [hint: it should be inside the loop]
    //print the final result inside the loop
    return 0;
}