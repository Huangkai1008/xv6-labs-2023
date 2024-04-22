#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 35

void sieve(int fd) {
    int i, j;
    char numbers[MAX_NUM + 1];
    for (i = 2; i <= MAX_NUM; i++) {
        numbers[i] = 1;
    }
    for (i = 2; i * i <= MAX_NUM; i++) {
        if (numbers[i] == 1) {
            for (j = i * i; j <= MAX_NUM; j += i) {
                numbers[j] = 0;
            }
        }
    }
    for (i = 2; i <= MAX_NUM; i++) {
        if (numbers[i] == 1) {
            write(fd, &i, sizeof(i));
        }
    }
    close(fd);
    exit(0);
}

int main() {
    int p[2];
    pipe(p);

    for (int i = 2; i <= MAX_NUM; i++) {
        write(p[1], &i, sizeof(i));
    }

    switch (fork()) {
        case -1:
            exit(-1);
        case 0:
            close(p[0]);
            sieve(p[1]);
        default:
            close(p[1]);
            int prime;
            while (read(p[0], &prime, sizeof(prime))) {
                printf("prime %d\n", prime);
            }
            close(p[0]);
            wait(0);
    }

    exit(0);
}
