#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Welcome to the Enchanted Fork and Exec Journey! \n");
    printf("Choose an element to transform into: \n");
    printf("1. Fire\n2. Water\n3. Earth\n4. Air\n");


    printf("Parent PID: %d\n", getpid());
    int choice;
    scanf("%d", &choice);

    pid = fork();

    if (pid < 0) 
    {
        perror("Fork Failed");
        return 1;
    } else if (pid == 0)
    {
        /* code */
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        //Execute based on numeric choice

        switch (choice)
        {
        case 1:
            execl("./fire_elemental","fire_elemental",NULL);
            break;
        case 2:
            execl("./water_elemental","water_elemental",NULL);
            break;
        case 3:
            execl("./earth_elemental","earth_elemental",NULL);
            break;
        case 4:
            execl("./air_elemental","air_elemental",NULL);
            break;

        default:
        fprintf(stderr, "Invalid choice. Exiting...\n");
        exit(1);
        }
    perror("Exec failed");
    exit(1);
    }
    else{
        wait(NULL);
        printf("Child process has completed.\n");
    }
    
    return 0;
}