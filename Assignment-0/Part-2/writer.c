#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("newfile.txt", "w");
    if (!fp) {
        perror("fopen failed");
        exit(1);
    }

    fprintf(fp, "Parent PID: %d\n", getppid());

    if (fclose(fp) != 0) {
        perror("fclose failed");
        exit(1);
    }

    return 0;
}
