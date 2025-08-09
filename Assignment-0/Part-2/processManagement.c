#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid1, pid2;
    printf("Running Task 1:\n");
    int x = 25;
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid1 == 0) {
        x += 10;
        printf("Child Process: PID = %d, x = %d\n", getpid(), x);
        exit(0); // Child from Task 1 stops here — will not run Task 2 or 3
    } 
    else {
        x -= 5;
        printf("Parent Process: PID = %d, x = %d\n", getpid(), x);
        wait(NULL); // Wait for Task 1 child to finish
    }

    // ===== Task 2 =====
    printf("\nRunning Task 2:\n");
    pid2 = fork();

    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid2 == 0) {
        execlp("./writer", "writer", NULL);
        perror("execlp failed");
        exit(1);
    } 
    else {
        wait(NULL);
        printf("Parent process (PID = %d) finished waiting.\n", getpid());
    }

    // ===== Task 3 =====
    printf("\nRunning Task 3:\n");
    pid_t pid3 = fork();

    if (pid3 < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid3 == 0) {
        sleep(2);
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        exit(0);
    } 
    else {
        printf("Parent Process: PID = %d\n", getpid());
        exit(0); // Parent terminates immediately — orphaning child
    }

}
